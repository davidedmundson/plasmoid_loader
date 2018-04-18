#include "proxywindow.h"

#include <KWayland/Server/surface_interface.h>
#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/buffer_interface.h>

#include "compositor.h"
#include "embeddedwindow.h"

using namespace KWayland;

ProxyWindow::ProxyWindow(KWayland::Server::ShellSurfaceInterface *ssi):
    QQuickWindow()
{
    auto surfaceItem = new SurfaceItem(contentItem());
    surfaceItem->setSurface(ssi);
    resize(surfaceItem->size().toSize());

    //DAVE - thiis is shit. Use one signal, and use vars for request / actual
    connect(surfaceItem, &SurfaceItem::widthChanged, this, [this]() {
        resize(m_surface->width(), m_surface->height());
    });
    connect(surfaceItem, &SurfaceItem::heightChanged, this, [this]() {
        resize(m_surface->width(), m_surface->height());
    });
//    ssi->transientFor();

    //resize the item to the window's size()
}
