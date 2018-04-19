#pragma once

#include <QObject>
#include <QHash>

namespace KWayland {
namespace  Server {
class SeatInterface;
class SurfaceInterface;
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

class Container {
public:
    Container();
    virtual ~Container();
    virtual QWindow* containerWindow() = 0;
    virtual QPoint adjustContainerOffset(const QPoint &offset) const = 0 ;
};


class Compositor: public QObject
{
    Q_OBJECT
public:
    static Compositor* self();
    Compositor();

    KWayland::Server::SeatInterface* seatInterface();
    Container* findContainer(KWayland::Server::SurfaceInterface *si);
    void registerContainer(Container *, KWayland::Server::SurfaceInterface *si);

Q_SIGNALS:
     void newSurface(KWayland::Server::ShellSurfaceInterface *ssi);

private:
    KWayland::Server::SeatInterface *m_seatIface;
    QHash<KWayland::Server::SurfaceInterface*, Container*> m_windows;

    bool m_hack = false;;
};


