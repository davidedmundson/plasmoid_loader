#include "proxywindow.h"

#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/buffer_interface.h>

#include <KWayland/Client/surface.h>
#include <KWayland/Client/shell.h>


ProxyWindow::ProxyWindow(KWayland::Server::ShellSurfaceInterface *ssi, QObject *parent):
    m_ssi(ssi)
{
    auto si = ssi->surface();


//    Shell *s = registry->createShell(name, version);

}
