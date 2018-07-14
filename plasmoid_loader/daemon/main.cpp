#include <QApplication>
#include <QDBusConnection>
#include "abstractappletfactory.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    AbstractAppletFactory f;
    QDBusConnection::sessionBus().registerService("org.plasma.provider.workspace");
    app.exec();
}
