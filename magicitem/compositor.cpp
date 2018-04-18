#include "compositor.h"

#include <KWayland/Server/display.h>
#include <KWayland/Server/output_interface.h>
#include <KWayland/Server/compositor_interface.h>
#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/datadevicemanager_interface.h>
#include <KWayland/Server/server_decoration_interface.h>

#include <KWayland/Client/compositor.h>
#include <KWayland/Client/shell.h>
#include <KWayland/Client/seat.h>
#include <KWayland/Client/shm_pool.h>
#include <KWayland/Client/registry.h>
#include <KWayland/Client/connection_thread.h>

#include <QApplication>

#include "proxywindow.h"

using namespace KWayland;

Q_GLOBAL_STATIC(Compositor, s_instance)

Compositor* Compositor::self() {
    return s_instance;
}


Compositor::Compositor() {
    //setup client stuff
    m_compositor = Client::Compositor::fromApplication(qApp);
    m_connectionThread = Client::ConnectionThread::fromApplication(qApp);
    Client::Registry registry;
    registry.create(m_connectionThread);
    registry.setup();
    connect(&registry, &Client::Registry::interfaceAnnounced, this, [this, &registry]() {
        Client::Registry::AnnouncedInterface t;

        t = registry.interface(Client::Registry::Interface::Seat);
        m_seat = registry.createSeat(t.name, t.version);

    });
    m_connectionThread->roundtrip();

    //setup server stuff
    auto displayIface = new Server::Display(this);
    displayIface->setSocketName(QString("plasma0"));
    displayIface->start();
    displayIface->createShm();

    auto shellIface = displayIface->createShell(displayIface);
    auto compositorIface = displayIface->createCompositor(this);
    auto decorationIface = displayIface->createServerSideDecorationManager(displayIface);
    decorationIface->setDefaultMode(Server::ServerSideDecorationManagerInterface::Mode::Server);

    auto outputIface = displayIface->createOutput(displayIface);
    const QSize initialSize(1920, 1080);
    outputIface->addMode(initialSize);
    outputIface->setModel("Monitor A");
    outputIface->setCurrentMode(initialSize);
    outputIface->create();

    m_seatIface = displayIface->createSeat(displayIface);
    m_seatIface->setHasKeyboard(true);
    m_seatIface->setHasPointer(true);
    auto dataIface = displayIface->createDataDeviceManager(displayIface);

    connect(shellIface, &Server::ShellInterface::surfaceCreated, this, [this](Server::ShellSurfaceInterface *ssi) {
        if (!m_hack) {
            emit newSurface(ssi);
        }
        new ProxyWindow(ssi, this);
    });

    compositorIface->create();
    m_seatIface->create();
    shellIface->create();
    dataIface->create();
}

Server::SeatInterface *Compositor::seatInterface()
{
    return m_seatIface;
}
