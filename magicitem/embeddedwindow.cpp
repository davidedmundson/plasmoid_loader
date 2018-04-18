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
    m_ssi->requestSize(QSize(width(), height()));

    connect(si, &SurfaceInterface::damaged, this, [si, this]() {
        m_timer.restart();
        if (!si->buffer()) {
            m_image = QImage();
            update();
        }
        m_image = QImage(); //free image that is now transferred. TODO front/back?
        m_image = si->buffer()->data();
        si->resetTrackedDamage();
        update();
        qDebug() << m_image.size();
    });
}

void SurfaceItem::paint(QPainter *painter)
{
    painter->drawImage(0, 0, m_image);
    if (m_si)
        m_si->frameRendered(m_timer.elapsed());
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
    if (!m_seat) return;
    m_seat->setTimestamp(event->timestamp());
    m_seat->setPointerPos(event->pos());
}

void SurfaceItem::mousePressEvent(QMouseEvent *event)
{
    if (!m_seat) return;
    m_seat->setTimestamp(event->timestamp());
    m_seat->setPointerPos(event->pos());
    m_seat->pointerButtonPressed(event->button());
}

void SurfaceItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_seat) return;
    m_seat->setTimestamp(event->timestamp());
    m_seat->pointerButtonReleased(event->button());
}

void SurfaceItem::keyPressEvent(QKeyEvent *event)
{
    const int magicOffset = KWindowSystem:: isPlatformX11() ? 8 : 0;
    m_seat->setTimestamp(event->timestamp());
    m_seat->keyPressed(event->nativeScanCode() - magicOffset);
}

void SurfaceItem::keyReleaseEvent(QKeyEvent *event)
{
    //obvious bug alert? - what if the key is released whilst this QQuickItem doesn't have focus?
    if (!m_seat) return;
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
