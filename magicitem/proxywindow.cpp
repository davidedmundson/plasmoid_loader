#include "proxywindow.h"

#include <KWayland/Server/surface_interface.h>
#include <KWayland/Server/xdgshell_interface.h>
#include <KWayland/Server/buffer_interface.h>

#include "compositor.h"
#include "embeddedwindow.h"

using namespace KWayland;

ProxyWindow::ProxyWindow(KWayland::Server::XdgShellSurfaceInterface *ssi):
    QQuickWindow()
{
    m_surface = new SurfaceItem(contentItem());
    m_surface->setSurface(ssi);
//     m_surface->setVisible(true);
    m_surface->setFocus(true);
    resize(m_surface->size().toSize());

    //DAVE - thiis is shit. Use one signal, and use vars for request / actual
    connect(m_surface, &SurfaceItem::widthChanged, this, [this]() {
        resize(m_surface->width(), m_surface->height());
    });
    connect(m_surface, &SurfaceItem::heightChanged, this, [this]() {
        resize(m_surface->width(), m_surface->height());
    });

    connect(m_surface, &SurfaceItem::hasBufferChanged, this, [this](bool hasBuffer) {
        if (hasBuffer) {
            show();
        } else {
            hide();
        }
    });

    setTitle(ssi->title());
    connect(ssi, &Server::XdgShellSurfaceInterface::titleChanged, this, &QWindow::setTitle);

//             setFlags(flags() | Qt::Popup);

//     connect(ssi, &Server::ShellSurfaceInterface::transientForChanged, this, [this, ssi]() {
//         Container *container = Compositor::self()->findContainer(ssi->transientFor().data());
//         if (!container) {
//             return;
//         }
//         setTransientParent(container->containerWindow());
//         QPoint offset = container->adjustContainerOffset(ssi->transientOffset());
//         setPosition(offset + container->containerWindow()->position());
//     });
//
//     connect(ssi, &Server::ShellSurfaceInterface::transientOffsetChanged, this, [this, ssi]() {
//         Container *container = Compositor::self()->findContainer(ssi->transientFor().data());
//         if (!container) {
//             return;
//         }
//         QPoint offset = container->adjustContainerOffset(ssi->transientOffset());
//         setPosition(offset + container->containerWindow()->position());
//     });

    connect(ssi, &Server::Resource::aboutToBeUnbound, this, &QWindow::close);
}

QWindow *ProxyWindow::containerWindow()
{
    return this;
}

QPoint ProxyWindow::adjustContainerOffset(const QPoint &offset) const
{
    return offset;
}
