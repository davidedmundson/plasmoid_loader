#include "abstractappletfactory.h"

#include <QDBusConnection>

AbstractAppletFactory::AbstractAppletFactory(QObject *parent):
    QObject(parent)
{
    m_id = "net.dave.awesomeApplet";
//     m_id.replace('.', '_'); //are dots

    new AppletFactoryAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/Plamsa/Factory/" + m_id);

}

QDBusObjectPath AbstractAppletFactory::CreateApplet(int id)
{
    AbstractAppletFactory applet = createApplet(id);
    return applet->path();
}

AbstractAppletFactory::createApplet(int id)
{
    if (m_applet.contains(id))  {
//         return m_applets[id]->path();
    }

    auto applet = new AbstractAppletFactory(int id, this);
    m_applets.insert(id, applet);
    return applet->path();
}
