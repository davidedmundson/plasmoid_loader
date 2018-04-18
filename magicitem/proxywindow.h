#pragma once
#include <QQuickWindow>

namespace KWayland {
namespace  Server {
class SeatInterface;
class SurfaceInterface;
class ShellSurfaceInterface; //TODO XDG
}
}

class SurfaceItem;

/**
 * This class is repsonsible for proxying all events from a surface to a new window
 *
 * transientFor and transientOffset are mapped specially. Rest should be roughly 1:1
 *
 */

class ProxyWindow: public QQuickWindow
{
    Q_OBJECT
public:
    ProxyWindow(KWayland::Server::ShellSurfaceInterface *ssi);

private:
    SurfaceItem *m_surface;
};
