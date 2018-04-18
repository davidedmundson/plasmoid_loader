#pragma once

#include <QObject>

namespace KWayland {
namespace  Server {
class SeatInterface;
class ShellSurfaceInterface;
}
namespace Client {
class Shell;
class Compositor;
class ShmPool;
class Seat;
class ConnectionThread;
}
}

class Compositor: public QObject
{
    Q_OBJECT
public:
    static Compositor* self();
    Compositor();

    KWayland::Server::SeatInterface* seatInterface();

    KWayland::Client::Shell *shell();
    KWayland::Client::Compositor *compositor();
    KWayland::Client::ShmPool *shmPool();
Q_SIGNALS:
     void newSurface(KWayland::Server::ShellSurfaceInterface *ssi);

private:
    KWayland::Server::SeatInterface *m_seatIface;

    KWayland::Client::Shell *m_shell;
    KWayland::Client::Compositor *m_compositor;
    KWayland::Client::ShmPool *m_shmPool;
    KWayland::Client::Seat *m_seat;
    KWayland::Client::ConnectionThread *m_connectionThread;

    bool m_hack = false;;
};
