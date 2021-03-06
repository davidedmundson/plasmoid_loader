#include "proxywindow.h"

#include <KWayland/Server/surface_interface.h>
#include <KWayland/Server/xdgshell_interface.h>
#include <KWayland/Server/seat_interface.h>
#include <KWayland/Server/buffer_interface.h>

#include "compositor.h"
#include "embeddedwindow.h"

using namespace KWayland;

AbstractProxyWindow::AbstractProxyWindow():
    QQuickWindow()
{
    m_surfaceItem = new SurfaceItem(contentItem());
    m_surfaceItem->setFocus(true);

    connect(m_surfaceItem, &SurfaceItem::hasBufferChanged, this, [this](bool hasBuffer) {
        if (hasBuffer) {
            show();
        } else {
            hide();
        }
    });
}

void AbstractProxyWindow::hideEvent(QHideEvent *e)
{
    emit closed();
}

QWindow *AbstractProxyWindow::containerWindow()
{
    return this;
}

QPoint AbstractProxyWindow::adjustContainerOffset(const QPoint &offset) const
{
    return offset;
}

TopLevelProxyWindow::TopLevelProxyWindow(KWayland::Server::XdgShellSurfaceInterface *toplevel)
{
    m_surfaceItem->setSurface(toplevel->surface());

    connect(toplevel, &Server::Resource::aboutToBeUnbound, this, &QWindow::close);

    setTitle(toplevel->title() + "(Proxy)");
//     connect(toplevel, &Server::XdgShellSurfaceInterface::titleChanged, this, &QWindow::setTitle);

    connect(m_surfaceItem, &SurfaceItem::sizeChanged, this, [this, toplevel]() {
        toplevel->configure(0, m_surfaceItem->size().toSize());
    });
    connect(this, &AbstractProxyWindow::closed, toplevel, [toplevel]() {
        qDebug() << "toplevel done";
//        toplevel->close();
    });
//    toplevel->configure(0, m_surfaceItem->size().toSize());
}

PopupProxyWindow::PopupProxyWindow(KWayland::Server::XdgShellPopupInterface *popup)
{

    m_surfaceItem->setSurface(popup->surface());

    Container *container = Compositor::self()->findContainer(popup->transientFor().data());
    if (container) {
        setTransientParent(container->containerWindow());
        setPosition(container->adjustContainerOffset(popup->transientOffset()) + container->containerWindow()->position());
        popup->configure(QRect(popup->transientOffset(), popup->initialSize()));
    } else {
    }

    setFlags(Qt::Popup);
    resize(popup->initialSize());

    connect(popup, &Server::XdgShellPopupInterface::grabRequested, this, [this]() {
        qDebug() << "grabbed!";
        setFlags(Qt::Popup);
    });

    connect(popup, &Server::Resource::aboutToBeUnbound, this, &QWindow::close);
    connect(this, &AbstractProxyWindow::closed, popup, [popup]() {
        qDebug() << "popup done";
        popup->popupDone();
    });
}
