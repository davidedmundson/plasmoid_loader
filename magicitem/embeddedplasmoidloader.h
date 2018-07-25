#pragma once

#include "embeddedwindow.h"

//this class is the publicly facable invocation of SurfaceItem
//it is responsible for launching a plasmoid over our DBus interface and handling relevant
//plasmoid specific things


//long term, maybe this shouldn't exist, we make our wrapper C++ and have this as the _graphics_item?

class EmbeddedPlasmoidLoader: public SurfaceItem
{
    Q_OBJECT
    Q_PROPERTY(qreal minimumWidth READ minimumWidth NOTIFY minimumSizeChanged)
    Q_PROPERTY(qreal minimumHeight READ minimumHeight NOTIFY minimumSizeChanged)

public:
    EmbeddedPlasmoidLoader(QQuickItem *parent = nullptr);
    Q_INVOKABLE void setTargetPlugin(const QString &targetPlugin);

    qreal minimumWidth() const;
    qreal minimumHeight() const;
signals:
    void minimumSizeChanged();
private:
    void setContents(KWayland::Server::XdgShellSurfaceInterface *shell);

private:
    KWayland::Server::XdgShellSurfaceInterface *m_shell;
    QSize m_minSize;

};
