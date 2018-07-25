#include <QApplication>
#include <QDBusConnection>
#include <QQuickWindow>

#include <KPackage/KPackage/PackageLoader>
#include <kplugininfo.h>
#include "plasma5appletfactory.h"

int main(int argc, char** argv)
{
    qputenv("QT_QPA_PLATFORM", "wayland");
    qputenv("WAYLAND_DISPLAY", "plasma0");
    qputenv("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1");
    QQuickWindow::setDefaultAlphaBuffer(true);

    QApplication app(argc, argv);

    QStringList applets = {"org.kde.plasma.digitalclock", "org.kde.plasma.analogclock", "org.kde.plasma.notes"};

    for(const QString &applet: applets) {
        new Plasma5AppletFactory(applet, &app);
    }

    qDebug() << "Registering service: org.plasma.provider.workspace";
    QDBusConnection::sessionBus().registerService("org.plasma.provider.workspace");
    app.exec();
}
