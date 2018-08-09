#include "embeddedwindow.h"

#include <KWayland/Server/display.h>
#include <KWayland/Server/output_interface.h>
#include <KWayland/Server/compositor_interface.h>
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/xdgshell_interface.h>
#include <KWayland/Server/seat_interface.h>
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

typedef GLboolean(*eglBindWaylandDisplayWL_func) (EGLDisplay dpy, wl_display *display);
typedef GLboolean(*eglQueryWaylandBufferWL_func) (EGLDisplay dpy, struct wl_resource *buffer, EGLint attribute, EGLint *value);
typedef EGLImageKHR(*eglCreateImageKHR_func) (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list);
typedef GLboolean(*eglImageTargetTexture2DOES_func) (int, GLeglImageOES);


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

void SurfaceItem::setSurface(SurfaceInterface *si)
{
    m_si = si;
    m_seat->setFocusedPointerSurface(si);
    m_seat->setFocusedKeyboardSurface(si);

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
        setWidth(si->buffer()->size().width());
        setHeight(si->buffer()->size().height());

        m_minSize = si->buffer()->size();
        update();

       si->resetTrackedDamage();

        update();
    });

    connect(this, &QQuickItem::activeFocusChanged, this, [this, si](bool activeFocus) {
        //DAVE - need to move this more globally to handle no-one having focus
        if (activeFocus) {

            qDebug() << this << "focus gained!";
        } else {
            qDebug() << this << "focus lost!";

        }
    });
}

SurfaceInterface *SurfaceItem::surface() const
{
    return m_si;
}

QSGNode* SurfaceItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (!m_si || ! m_si->buffer()) {
        delete oldNode;
        return nullptr;
    }

    auto  currentNode = static_cast<QSGSimpleTextureNode*>(oldNode);
    if (!currentNode) {
        currentNode = new QSGSimpleTextureNode();
    }

    auto b  = m_si->buffer();

    GLuint newTexture = 0;

    if (b->shmBuffer()) {
        glGenTextures(1, &newTexture);
        auto image = b->data();
        image.save("/tmp/foo.png");
        glBindTexture( GL_TEXTURE_2D, newTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.size().width(), image.size().height(), 0,
                GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    } else {
        //      TODO only lookup once
       auto eglQueryWaylandBufferWL = (eglQueryWaylandBufferWL_func)eglGetProcAddress("eglQueryWaylandBufferWL");
       auto eglCreateImageKHR = (eglCreateImageKHR_func)eglGetProcAddress("eglCreateImageKHR");
       auto glEGLImageTargetTexture2DOES = (eglImageTargetTexture2DOES_func)eglGetProcAddress("glEGLImageTargetTexture2DOES");

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
        QSGTexture *source = window()->createTextureFromId(newTexture, m_minSize, QQuickWindow::TextureHasAlphaChannel);
        source->setFiltering(QSGTexture::Linear);
        currentNode->setTexture(source);
        currentNode->setOwnsTexture(true);

        const QRect destRect(QPointF(boundingRect().center() - QPointF(m_minSize.width(), m_minSize.height()) / 2).toPoint(), m_minSize);
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
    emit sizeChanged();
}

void SurfaceItem::hoverMoveEvent(QHoverEvent *event)
{
    if (Compositor::self()->activeClient() != m_si) {
        return;
    }
    m_seat->setTimestamp(event->timestamp());
    m_seat->setPointerPos(event->pos());
}

void SurfaceItem::mousePressEvent(QMouseEvent *event)
{
    setFocus(true);
    Compositor::self()->setActiveClient(m_si);

    m_seat->setTimestamp(event->timestamp());
    m_seat->setPointerPos(event->pos());
    m_seat->pointerButtonPressed(event->button());
}

void SurfaceItem::mouseMoveEvent(QMouseEvent *event)
{
    if (Compositor::self()->activeClient() != m_si) {
        return;
    }
    m_seat->setTimestamp(event->timestamp());
    m_seat->setPointerPos(event->pos());
}

void SurfaceItem::mouseReleaseEvent(QMouseEvent *event)
{
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
    const int magicOffset = KWindowSystem:: isPlatformX11() ? 8 : 0;
    m_seat->setTimestamp(event->timestamp());
    m_seat->keyReleased(event->nativeScanCode() - magicOffset);
}

SurfaceItem::SurfaceItem(QQuickItem *parent):
    QQuickItem(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setActiveFocusOnTab(true);
    setFlag(ItemHasContents);

    Compositor::self(); //start compositor if it's not already
    connect(this, &SurfaceItem::hasBufferChanged, this, &QQuickItem::update);
    m_seat = Compositor::self()->seatInterface();
}

SurfaceItem::~SurfaceItem()
{
}

void SurfaceItem::wheelEvent(QWheelEvent *event)
{
    if (Compositor::self()->activeClient() != m_si) {
        return;
    }
    m_seat->setTimestamp(event->timestamp());
    const QPoint &angle = event->angleDelta() / (8 * 15);
    if (angle.x() != 0) {
        m_seat->pointerAxis(Qt::Horizontal, - angle.x() * 120);
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


TestItem::TestItem(QQuickItem *parent)
    :SurfaceItem(parent)
{
    connect(Compositor::self(), &Compositor::newSurface, this, [=](KWayland::Server::XdgShellSurfaceInterface *toplevel) {
        if (surface()) {
            return;
        }

        qDebug() << "test item";
        setSurface(toplevel->surface());
        toplevel->configure(0, size().toSize());
        Compositor::self()->registerContainer(this, toplevel->surface());

        connect(this, &SurfaceItem::sizeChanged, toplevel, [=]() {
            toplevel->configure(0, size().toSize());
        });
    });
}
