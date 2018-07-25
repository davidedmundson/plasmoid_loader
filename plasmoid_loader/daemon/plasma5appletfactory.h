#include "abstractappletfactory.h"

class Plasma5AppletWrapper;
class PlasmaWindowedCorona;

/*
 * This is a factory class to wrap the current Plasma 5 API
 */
class Plasma5AppletFactory: public AbstractAppletFactory
{
public:
    Plasma5AppletFactory(const QString &pluginId, QObject *parent);
    AbstractAppletWrapper* createApplet(int id) override;
private:
    const QString m_pluginId; //as in org.kde.plasma.digitalclock / whatever
    PlasmaWindowedCorona *m_corona;
};
