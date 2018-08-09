#pragma once

#include <QQuickPaintedItem>
#include <QPointer>
#include <QElapsedTimer>
#include <QPixmap>


namespace KWayland {
namespace  Server {
class SeatInterface;
class SurfaceInterface;
class XdgShellSurfaceInterface;
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
    void setSurface(KWayland::Server::SurfaceInterface  *surface);
    KWayland::Server::SurfaceInterface* surface() const;
    QPointer<KWayland::Server::SeatInterface> m_seat;

    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    QWindow *containerWindow() override;
    QPoint adjustContainerOffset(const QPoint &point) const override;

signals:
    void hasBufferChanged(bool hasBuffer);
    void sizeChanged();
protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    QPointer<KWayland::Server::SurfaceInterface> m_si;
private:
    bool m_hasBuffer = false;
    QElapsedTimer m_timer;
    QSize m_minSize;
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
