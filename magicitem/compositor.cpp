#include "compositor.h"

#include <KWayland/Server/display.h>
#include <KWayland/Server/output_interface.h>
#include <KWayland/Server/compositor_interface.h>
#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/datadevicemanager_interface.h>
#include <KWayland/Server/server_decoration_interface.h>

#include <QApplication>
#include <QThread>

#include "proxywindow.h"
#include "embeddedwindow.h"

using namespace KWayland;

Q_GLOBAL_STATIC(Compositor, s_instance)

Compositor* Compositor::self() {
    return s_instance;
}

Compositor::Compositor() {
    //setup server stuff
    auto displayIface = new Server::Display(this);
    displayIface->setSocketName(QString("plasma0"));
    displayIface->start();
    displayIface->createShm();
    SurfaceItem::initialiseDisplay(displayIface);

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
//    m_seatIface->setHasKeyboard(true);
//    m_seatIface->setKeymap(); //DAVE, boring TODO
    m_seatIface->setHasPointer(true);
    auto dataIface = displayIface->createDataDeviceManager(displayIface);

    connect(shellIface, &Server::ShellInterface::surfaceCreated, this, [this](Server::ShellSurfaceInterface *ssi) {
        if (!m_hack) {
            qDebug() << "new embed";
            emit newSurface(ssi);
            m_hack = true;

            return;
        }
        qDebug() << "new proxy";
        new ProxyWindow(ssi); //responsible for deleting itself kjob style
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

Container *Compositor::findContainer(Server::SurfaceInterface *si)
{
    return m_windows[si];
}

void Compositor::registerContainer(Container *container, KWayland::Server::SurfaceInterface *si)
{
    m_windows[si] = container;
}



Container::Container(){}
Container::~Container(){}
