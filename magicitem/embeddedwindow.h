#pragma once

#include <QQuickPaintedItem>
#include <QPointer>
#include <QElapsedTimer>
#include <QPixmap>


namespace KWayland {
namespace  Server {
class SeatInterface;
class SurfaceInterface;
class ShellSurfaceInterface; //TODO XDG
class Display;
}
}

#include "compositor.h"

/*
 * Renders a surface as a QQuickItem
 */
class SurfaceItem : public QQuickItem, public Container
{
    Q_OBJECT
public:
    SurfaceItem(QQuickItem *parent=0);
    ~SurfaceItem();
//    virtual void paint(QPainter *painter) override; //this sucks, do it properly with GL too, see QtQuick wayland and your demo

    void setSurface(KWayland::Server::ShellSurfaceInterface *surface);
    QPointer<KWayland::Server::SeatInterface> m_seat;

    QWindow *containerWindow() override;
    QPoint adjustContainerOffset(const QPoint &point) const override;

    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

    static void initialiseDisplay(KWayland::Server::Display *display);
signals:
    void hasBufferChanged(bool hasBuffer);
protected:
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    QPointer<KWayland::Server::SurfaceInterface> m_si;
    QPointer<KWayland::Server::ShellSurfaceInterface> m_ssi;
private:
    bool m_hasBuffer = false;
    QElapsedTimer m_timer;
    QSize m_size;
};
