#include "abstractappletfactory.h"
#include "appletfactoryadaptor.h"

#include "abstractappletwrapper.h"

#include <QDBusConnection>

AbstractAppletFactory::AbstractAppletFactory(const QString &id, QObject *parent):
    QObject(parent),
    m_id(id)
{
    m_id.replace('.', '_'); //are dots

    new AppletFactoryAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/Plasma/Factory/" + m_id, this);
}

AbstractAppletFactory::~AbstractAppletFactory()
{
}

QDBusObjectPath AbstractAppletFactory::GetApplet(int id)
{
    auto applet = createApplet(id);
    return QDBusObjectPath(applet->path());
}

AbstractAppletWrapper* AbstractAppletFactory::createApplet(int id)
{
    if (m_applets.contains(id))  {
//         return m_applets[id]->path();
    }

    auto applet = new AbstractAppletWrapper(id, this);
    m_applets.insert(id, applet);
    return applet;
}
