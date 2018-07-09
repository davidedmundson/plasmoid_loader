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
    static void initialiseDisplay(KWayland::Server::Display *display);

    SurfaceItem(QQuickItem *parent=0);
    ~SurfaceItem();
    void setSurface(KWayland::Server::ShellSurfaceInterface *surface);
    KWayland::Server::ShellSurfaceInterface* surface() const;
    QPointer<KWayland::Server::SeatInterface> m_seat;

    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    QWindow *containerWindow() override;
    QPoint adjustContainerOffset(const QPoint &point) const override;

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

// class EmbeddedWindow: public SurfaceItem, public Container
// {
    //EmbeddedWindow(QQuickItem *parent = nullptr);
//     QWindow *containerWindow() override;
//     QPoint adjustContainerOffset(const QPoint &point) const override;
// }


/*Claims the first window set on this compositor - used for testing only*/
class TestItem : public SurfaceItem
{
    Q_OBJECT
public:
    TestItem(QQuickItem *parent = 0);
};
