#include "proxywindow.h"

#include <KWayland/Server/surface_interface.h>
#include <KWayland/Server/xdgshell_interface.h>
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

    setTitle(toplevel->title());
    connect(toplevel, &Server::XdgShellSurfaceInterface::titleChanged, this, &QWindow::setTitle);

    //DAVE - thiis is shit. Use one signal, and use vars for request / actual
    connect(m_surfaceItem, &SurfaceItem::widthChanged, this, [this, toplevel]() {
        toplevel->configure(0, QSize(m_surfaceItem->width(), m_surfaceItem->height()));
    });
    connect(m_surfaceItem, &SurfaceItem::heightChanged, this, [this, toplevel]() {
        toplevel->configure(0, QSize(m_surfaceItem->width(), m_surfaceItem->height()));
    });
}

PopupProxyWindow::PopupProxyWindow(KWayland::Server::XdgShellPopupInterface *popup)
{
    setFlags(flags() | Qt::Popup);
    m_surfaceItem->setSurface(popup->surface());

    Container *container = Compositor::self()->findContainer(popup->transientFor().data());
    if (container) {
        setTransientParent(container->containerWindow());
        setPosition(popup->transientOffset() + container->containerWindow()->position());
    }
    connect(popup, &Server::Resource::aboutToBeUnbound, this, &QWindow::close);

}
