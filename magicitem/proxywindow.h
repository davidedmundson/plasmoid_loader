#pragma once
#include <QQuickWindow>

#include "compositor.h"

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

class ProxyWindow: public QQuickWindow, public Container
{
    Q_OBJECT
public:
    ProxyWindow(KWayland::Server::ShellSurfaceInterface *ssi);
    QWindow* containerWindow() override;
    QPoint adjustContainerOffset(const QPoint &offset) const override;
private:
    SurfaceItem *m_surface;
};
