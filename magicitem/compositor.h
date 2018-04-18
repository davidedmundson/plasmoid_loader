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

class QWindow;

class Compositor: public QObject
{
    Q_OBJECT
public:
    static Compositor* self();
    Compositor();

    KWayland::Server::SeatInterface* seatInterface();

Q_SIGNALS:
     void newSurface(KWayland::Server::ShellSurfaceInterface *ssi);

private:
    KWayland::Server::SeatInterface *m_seatIface;

    bool m_hack = false;;
};


class Container {
public:
    Container();
    virtual ~Container();
    virtual QWindow* containerWindow() = 0;
    virtual QPoint adjustOffset(const QPoint &offset) = 0 ;
};
