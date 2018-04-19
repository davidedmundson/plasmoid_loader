#include "embeddedwindow.h"

#include <KWayland/Server/display.h>
#include <KWayland/Server/output_interface.h>
#include <KWayland/Server/compositor_interface.h>
#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/datadevicemanager_interface.h>
#include <KWayland/Server/server_decoration_interface.h>
#include <KWindowSystem>

#include <QPainter>
#include <QQuickWindow>

#include "compositor.h"

using namespace KWayland::Server;

void SurfaceItem::setSurface(ShellSurfaceInterface *ssi)
{
    if (m_ssi) {
        //goal here is to only cope with one window.
        //if some muppet is sending two, that's their problem.
        return;
    }
    m_ssi = ssi;
    auto si = ssi->surface();
    m_seat = Compositor::self()->seatInterface();
    m_seat->setFocusedPointerSurface(si);
    m_seat->setFocusedKeyboardSurface(si);

    m_si = si;
    m_ssi->requestSize(QSize(width(),height()));

    connect(si, &SurfaceInterface::unmapped, this, [si, this]() {
        m_hasBuffer = false;
        qDebug() << "unmapped";
        emit hasBufferChanged(m_hasBuffer);
    });

    connect(si, &SurfaceInterface::unbound, this, [si, this]() {
        m_hasBuffer = false;
        qDebug() << "unbound";
        emit hasBufferChanged(m_hasBuffer);
    });
    connect(si, &SurfaceInterface::destroyed, this, [si, this]() {
        m_hasBuffer = false;
        qDebug() << "destroyed";
        emit hasBufferChanged(m_hasBuffer);
    });


    connect(si, &SurfaceInterface::damaged, this, [si, this]() {
        m_timer.restart();
        if (!si->buffer()) {
            m_hasBuffer = false;
            emit hasBufferChanged(m_hasBuffer);
            update();
        }
        m_hasBuffer = true;
        emit hasBufferChanged(m_hasBuffer);
        qDebug() << "setting size to " << si->buffer()->size();
        setWidth(si->buffer()->size().width());
        setHeight(si->buffer()->size().height());
        emit widthChanged();

        si->resetTrackedDamage();
        update();
    });

    connect(this, &QQuickItem::activeFocusChanged, this, [this, si](bool activeFocus) {
        //DAVE - need to move this more globally to handle no-one having focus
        if (activeFocus) {
            m_seat->setFocusedKeyboardSurface(si);
            m_seat->setFocusedPointerSurface(si);
            qDebug() << "focus lost";
        }
    });
}

void SurfaceItem::paint(QPainter *painter)
{
    if (m_si  && m_si->buffer()) {
        qDebug() << "painting";
        QImage image = m_si->buffer()->data();
        qDebug() << this << image.size();
        painter->drawImage(0, 0, image);
        m_si->frameRendered(m_timer.elapsed());
    }
}

void SurfaceItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_UNUSED(oldGeometry);
    if (!m_ssi) {
        return;
    }
    m_ssi->requestSize(newGeometry.size().toSize());
}

void SurfaceItem::hoverMoveEvent(QHoverEvent *event)
{
    if (!m_seat || !hasActiveFocus()) return;

    m_seat->setTimestamp(event->timestamp());
    m_seat->setPointerPos(event->pos());
}

static bool pressed=false;

void SurfaceItem::mousePressEvent(QMouseEvent *event)
{
    if (!m_seat || !hasActiveFocus()) return;
    m_seat->setTimestamp(event->timestamp());
    m_seat->setPointerPos(event->pos());
    if (!pressed)
        m_seat->pointerButtonPressed(event->button());
    pressed=true;
}

void SurfaceItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_seat || !hasActiveFocus()) return;
    m_seat->setTimestamp(event->timestamp());
    if (pressed)
        m_seat->pointerButtonReleased(event->button());
    pressed=false;
}

void SurfaceItem::keyPressEvent(QKeyEvent *event)
{
    if (!m_seat || !hasActiveFocus()) return;
    const int magicOffset = KWindowSystem:: isPlatformX11() ? 8 : 0;
    m_seat->setTimestamp(event->timestamp());
    m_seat->keyPressed(event->nativeScanCode() - magicOffset);
}

void SurfaceItem::keyReleaseEvent(QKeyEvent *event)
{
    if (!m_seat || !hasActiveFocus()) return;
    const int magicOffset = KWindowSystem:: isPlatformX11() ? 8 : 0;
    m_seat->setTimestamp(event->timestamp());
    m_seat->keyReleased(event->nativeScanCode() - magicOffset);
}

//void SurfaceItem::resizeEvent(QResizeEvent *e)
//{
//    m_surfaceItem->setSize(e->size());
//}

SurfaceItem::SurfaceItem(QQuickItem *parent):
    QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setActiveFocusOnTab(true);

    Compositor::self(); //start compositor if it's not already
    connect(Compositor::self(), &Compositor::newSurface, this, &SurfaceItem::setSurface);
}

SurfaceItem::~SurfaceItem()
{
}

void SurfaceItem::wheelEvent(QWheelEvent *event)
{
    m_seat->setTimestamp(event->timestamp());
    const QPoint &angle = event->angleDelta() / (8 * 15);
    if (angle.x() != 0) {
        m_seat->pointerAxis(Qt::Horizontal, angle.x() * 120);
    }
    if (angle.y() != 0) {
        m_seat->pointerAxis(Qt::Vertical, -angle.y() * 120);
    }
}

QWindow *SurfaceItem::containerWindow()
{
    return this->window();
}

QPoint SurfaceItem::adjustContainerOffset(const QPoint &point) const
{
    return this->mapToGlobal(point).toPoint();
}
