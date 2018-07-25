#include "plasma5appletfactory.h"
#include "abstractappletwrapper.h"

Plasma5AppletFactory::Plasma5AppletFactory(const QString &pluginId, QObject *parent):
    AbstractAppletFactory(pluginId, parent)
{

}

AbstractAppletWrapper *Plasma5AppletFactory::createApplet(int id)
{
    return new AbstractAppletWrapper(id, this);
}
