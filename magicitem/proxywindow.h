#pragma once
#include <QObject>

namespace KWayland {
namespace  Server {
class SeatInterface;
class SurfaceInterface;
class ShellSurfaceInterface; //TODO XDG
}
}

/**
 * This class is repsonsible for proxying all events from a surface to a new window
 *
 * transientFor and transientOffset are mapped specially. Rest should be roughly 1:1
 *
 */

class ProxyWindow: public QObject
{
    Q_OBJECT
public:
    ProxyWindow(KWayland::Server::ShellSurfaceInterface *ssi, QObject *parent);

private:
    KWayland::Server::ShellSurfaceInterface *m_ssi;
};
