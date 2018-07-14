#include "appletfactory.h"

#include <QDBusConnection>

AppletFactory::AppletFactory(QObject *parent):
    QObject(parent)
{
    m_id = "net.dave.awesomeApplet";
//     m_id.replace('.', '_'); //are dots

    new AppletFactoryAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/Plamsa/Factory/" + m_id);

}

QDBusObjectPath AppletFactory::CreateApplet(int id)
{
    AppletWrapper applet = createApplet(id);
    return applet->path();
}

AppletFactory::createApplet(int id)
{
    if (m_applet.contains(id))  {
//         return m_applets[id]->path();
    }

    auto applet = new AppletWrapper(int id, this);
    m_applets.insert(id, applet);
    return applet->path();
}
