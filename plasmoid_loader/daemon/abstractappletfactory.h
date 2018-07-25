#pragma once

#include <QObject>
#include <QDBusObjectPath>
#include <QHash>

class AbstractAppletWrapper;

//This class is responsible for creating new instances of *a* specifc applet id
class AbstractAppletFactory: public QObject
{
public:
    AbstractAppletFactory(const QString &name, QObject *parent = nullptr);
    ~AbstractAppletFactory();

    //DBus invoked
    QDBusObjectPath GetApplet(int id); //DAVE should restoring previous instances be here or separate?

    //DAVE - this should be AbstractAppletFactory  with applet construction as a virtual call
    //Then Plasma 5 wrapper can do one thing, we can still have custom C++ and whatever
    virtual AbstractAppletWrapper* createApplet(int id);

    QString id() const {return m_id;}

private:
    QString m_id;
    QHash<int /*id*/, AbstractAppletWrapper*> m_applets;
};
