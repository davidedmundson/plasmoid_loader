#include "embeddedwindow.h"

#include <KWayland/Server/display.h>
#include <KWayland/Server/output_interface.h>
#include <KWayland/Server/compositor_interface.h>
#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/datadevicemanager_interface.h>
#include <KWayland/Server/server_decoration_interface.h>
#include <KWindowSystem>

#include <QQuickWindow>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QGuiApplication>
#include <QtX11Extras/QX11Info>

#include <QSGSimpleTextureNode>

#include "compositor.h"
#include <qpa/qplatformnativeinterface.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

using namespace KWayland::Server;

//maybe we mod QtWayland to cover this case.. it already has all this nonsense

typedef GLboolean(*eglBindWaylandDisplayWL_func)(EGLDisplay dpy, wl_display *display);
typedef GLboolean(*eglQueryWaylandBufferWL_func)(EGLDisplay dpy, struct wl_resource *buffer, EGLint attribute, EGLint *value);
typedef EGLImageKHR (*eglCreateImageKHR_func) (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list);

#ifndef EGL_WAYLAND_BUFFER_WL
#define EGL_WAYLAND_BUFFER_WL                   0x31D5
#endif
#ifndef EGL_WAYLAND_PLANE_WL
#define EGL_WAYLAND_PLANE_WL                    0x31D6
#endif
#ifndef EGL_WAYLAND_Y_INVERTED_WL
#define EGL_WAYLAND_Y_INVERTED_WL               0x31DB
#endif



EGLDisplay display()
{
    //TODO runtime
   auto d = static_cast<EGLDisplay*>(qApp->platformNativeInterface()->nativeResourceForIntegration("egldisplay"));
//     auto d = eglGetDisplay(QX11Info::display());
    return d;
}

void SurfaceItem::initialiseDisplay(KWayland::Server::Display *displayiFace) {
    auto d = display();
    eglInitialize(d, NULL, NULL);
    auto eglBindWaylandDisplayWL = (eglBindWaylandDisplayWL_func)eglGetProcAddress("eglBindWaylandDisplayWL");
    qDebug() << "DAVE" << d << eglBindWaylandDisplayWL;
   eglBindWaylandDisplayWL(d, *displayiFace);
   displayiFace->setEglDisplay(d);
}

void SurfaceItem::setSurface(ShellSurfaceInterface *ssi)
{
    if (m_ssi) {
        //goal here is to only cope with one window.
        //if some muppet sets two, that's their problem.
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


    connect(si, &SurfaceInterface::damaged, this, [si, this](const QRegion &r) {
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

        m_size = si->buffer()->size();
        update();

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

QSGNode* SurfaceItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (!m_si || ! m_si->buffer()) {
        delete oldNode;
        return nullptr;
    }
    delete oldNode;

    QSGSimpleTextureNode* currentNode = 0;
//     auto  currentNode = static_cast<QSGSimpleTextureNode*>(oldNode);
    if (!currentNode) {
        currentNode = new QSGSimpleTextureNode();
    }

    auto b  = m_si->buffer();

    GLuint newTexture = 0;

    if (b->shmBuffer()) {
        qDebug() << "SHM PATH";
        glGenTextures(1, &newTexture);
        auto image = b->data();;
        glBindTexture( GL_TEXTURE_2D, newTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.size().width(), image.size().height(), 0,
                GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    } else {

        qDebug() << "GL PATH";

        //      TODO only lookup once
       auto eglQueryWaylandBufferWL = (eglQueryWaylandBufferWL_func)eglGetProcAddress("eglQueryWaylandBufferWL");
       auto eglCreateImageKHR = (eglCreateImageKHR_func)eglGetProcAddress("eglCreateImageKHR");

        glGenTextures(1, &newTexture);
        const EGLint attribs[] = {
           EGL_WAYLAND_PLANE_WL, 0,
           EGL_NONE
       };
       EGLImageKHR image = eglCreateImageKHR(display(), EGL_NO_CONTEXT, EGL_WAYLAND_BUFFER_WL,
                                             (EGLClientBuffer)b->resource(), attribs);

       if (!image) {
           qWarning() << "WTF!!! could not load EGL Buffer. Just disable egl stuff above and use SHM";
       }

       glBindTexture(GL_TEXTURE_2D, newTexture);
       glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)image);
    }

    if (newTexture) {
        QSGTexture *source = window()->createTextureFromId(newTexture, m_size);
        source->setFiltering(QSGTexture::Linear);
        currentNode->setTexture(source);
        currentNode->setOwnsTexture(true);

        const QRect destRect(QPointF(boundingRect().center() - QPointF(m_size.width(), m_size.height()) / 2).toPoint(), m_size);
        currentNode->setRect(destRect);

        if (m_si) {
            m_si->frameRendered(m_timer.elapsed());
        }
    }

    return currentNode;
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
    QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setActiveFocusOnTab(true);
    setFlag(ItemHasContents);

    Compositor::self(); //start compositor if it's not already
    connect(Compositor::self(), &Compositor::newSurface, this, &SurfaceItem::setSurface);
    connect(this, &SurfaceItem::hasBufferChanged, this, &QQuickItem::update);
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
