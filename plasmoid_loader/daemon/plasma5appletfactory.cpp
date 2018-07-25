#include "plasma5appletfactory.h"
#include "abstractappletwrapper.h"

#include "plasmawindowedcorona.h"

Plasma5AppletFactory::Plasma5AppletFactory(const QString &pluginId, QObject *parent):
    AbstractAppletFactory(pluginId, parent),
    m_corona(new PlasmaWindowedCorona(this))
{
    //DAVE TODO
    //plasmawindowedcorona.h needs splitting up
    //corona should be app wide
    //loadApplet code should be here in createApplet
    //plasmaappletwrapper should have a containment + applet (current plasmawindowedview)
    //then hook things up magically
}

AbstractAppletWrapper *Plasma5AppletFactory::createApplet(int zz)
{
    m_corona->loadApplet(id(), QVariantList());
    return new AbstractAppletWrapper(zz, this);
}
