#pragma once

#include <QObject>
#include <QHash>
#include <QPointer>

namespace KWayland {
namespace  Server {
class SeatInterface;
class SurfaceInterface;
class XdgShellSurfaceInterface;
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

    //TODO map/unmap
    void registerContainer(Container *, KWayland::Server::SurfaceInterface *si);

    void setActiveClient(KWayland::Server::SurfaceInterface *si);
    KWayland::Server::SurfaceInterface *activeClient() const;

Q_SIGNALS:
     void newSurface(KWayland::Server::XdgShellSurfaceInterface *shellTopLevel);

private:
    KWayland::Server::SeatInterface *m_seatIface;
    QHash<KWayland::Server::SurfaceInterface*, Container*> m_windows;
    QPointer<KWayland::Server::SurfaceInterface> m_active;
};


