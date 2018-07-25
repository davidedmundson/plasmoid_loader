#include "embeddedplasmoidloader.h"
#include "AppletFactoryIface.h"

#include "compositor.h"
#include <KWayland/Server/xdgshell_interface.h>

EmbeddedPlasmoidLoader::EmbeddedPlasmoidLoader(QQuickItem *parent)
{
    //TODO we need to map.. but we don't have any of the XDGShellSurface data yet!
    //need to modify the core design so that the compositor keeps all surfaces, notifies us when their state changes
    //and we make proxywindows create on mapping if no-one else has claimed them
    connect(Compositor::self(), &Compositor::newSurface, this, [=](KWayland::Server::XdgShellSurfaceInterface *ssi) {
        if (surface()) {
            return;
        }
        setContents(ssi);
    });
}

void EmbeddedPlasmoidLoader::setTargetPlugin(const QString &targetPlugin)
{
    QString path = targetPlugin;
    path.replace('.', '_');

    OrgKdePlasmaAppletFactoryInterface factory("org.plasma.provider.workspace", "/Plasma/Factory/" + path, QDBusConnection::sessionBus());

    qDebug() <<  "Creating applet";
    auto reply = factory.GetApplet(1);
    qDebug() << reply.isError();
}

qreal EmbeddedPlasmoidLoader::minimumWidth() const
{
    return m_minSize.width();
}

qreal EmbeddedPlasmoidLoader::minimumHeight() const
{
    return m_minSize.height();
}

void EmbeddedPlasmoidLoader::setContents(KWayland::Server::XdgShellSurfaceInterface *shell)
{
    m_shell = shell;
    setSurface(shell->surface());
    Compositor::self()->registerContainer(this, shell->surface());

    connect(this, &SurfaceItem::sizeChanged, shell, [=]() {
        m_shell->configure(0, size().toSize());
    });
    connect(shell, &KWayland::Server::XdgShellSurfaceInterface::minSizeChanged, this, [=](const QSize &size) {
        m_minSize = size;
        qDebug() << "minimum size == " << m_minSize;
        emit minimumSizeChanged();
    });

    shell->configure(0, size().toSize());
    emit minimumSizeChanged();
}


