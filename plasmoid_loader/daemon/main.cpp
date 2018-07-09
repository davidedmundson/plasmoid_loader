#include <QApplication>
#include <QDBusConnection>
#include "appletfactory.h"

int main(int argc, char** argv)
{
    QApplication app;
    AppletFactory f;
    QDBusConnection::sessionBus.registerService("org.plasma.provider.workspace");
    app.exec();
}
