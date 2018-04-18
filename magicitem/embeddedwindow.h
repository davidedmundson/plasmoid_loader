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
}
}

/*
 * Renders a surface as a QQuickItem
 */
class SurfaceItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    SurfaceItem(QQuickItem *parent=0);
    ~SurfaceItem();
    virtual void paint(QPainter *painter) override; //this sucks, do it properly with GL too, see QtQuick wayland and your demo

    void setSurface(KWayland::Server::ShellSurfaceInterface *surface);
    QPointer<KWayland::Server::SeatInterface> m_seat;
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
    QImage m_image;
    QElapsedTimer m_timer;
};
