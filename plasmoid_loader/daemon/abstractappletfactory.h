//This class is responsible for creating new instances of *a* specifc applet id

class AppletFactory: public QObject
{
    public:
        AppletFactory(QObject *parent = nullptr);
        ~AppletFactory();

//DBus invoked
        QDBusObjectPath GetApplet(int id); //DAVE should restoring previous instances be here or separate?

        //DAVE - this should be AbstractAppletFactory  with applet construction as a virtual call
        //Then Plasma 5 wrapper can do one thing, we can still have custom C++ and whatever
        AppletWrapper* createApplet(int id);


private:
    QString m_id;
    QHash<int /*id*/, AppletWrapper*> m_applets;
};
