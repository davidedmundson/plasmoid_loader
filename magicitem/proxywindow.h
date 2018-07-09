#pragma once
#include <QQuickWindow>

#include "compositor.h"

namespace KWayland {
namespace  Server {
class SeatInterface;
class SurfaceInterface;
class XdgShellSurfaceInterface;
class XdgShellPopupInterface;
}
}

class SurfaceItem;

/**
 * This class is repsonsible for proxying all events from a surface to a new window
 *
 * transientFor and transientOffset are mapped specially. Rest should be roughly 1:1
 *
 */

class AbstractProxyWindow: public QQuickWindow, public Container
{
    Q_OBJECT
public:
    AbstractProxyWindow();
    QWindow* containerWindow() override;
    QPoint adjustContainerOffset(const QPoint &offset) const override;
    void hideEvent(QHideEvent *e) override;

Q_SIGNALS:
    void closed();
protected:
    SurfaceItem *m_surfaceItem;
};

class TopLevelProxyWindow: public AbstractProxyWindow
{
public:
    TopLevelProxyWindow(KWayland::Server::XdgShellSurfaceInterface *toplevel);
};

class PopupProxyWindow: public AbstractProxyWindow
{
public:
    PopupProxyWindow(KWayland::Server::XdgShellPopupInterface *popup);
};


