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

    //DAVE - how do we want to do this?
    QStringList applets = {
        "org.kde.plasma.activitybar",
        "org.kde.plasma.activitypager",
        "org.kde.plasma.analogclock",
        "org.kde.plasma_applet_dict",
        "org.kde.plasma.appmenu",
        "org.kde.plasma.battery",
        "org.kde.plasma.binaryclock",
        "org.kde.plasma.bluetooth",
        "org.kde.plasma.calculator",
        "org.kde.plasma.calendar",
        "org.kde.plasma.clipboard",
        "org.kde.plasma.colorpicker",
        "org.kde.plasma.comic",
        "org.kde.plasma.devicenotifier",
        "org.kde.plasma.digitalclock",
        "org.kde.plasma.diskquota",
        "org.kde.plasma.fifteenpuzzle",
        "org.kde.plasma.folder",
        "org.kde.plasma.fuzzyclock",
        "org.kde.plasma.grouping",
        "org.kde.plasma.icon",
        "org.kde.plasma.icontasks",
        "org.kde.plasma.katesessions",
        "org.kde.plasma.keyboardindicator",
        "org.kde.plasma.kicker",
        "org.kde.plasma.kickerdash",
        "org.kde.plasma.kickoff",
        "org.kde.plasma.kimpanel",
        "org.kde.plasma.konsoleprofiles",
        "org.kde.plasma.lock_logout",
        "org.kde.plasma.mediacontroller",
        "org.kde.plasma.mediaframe",
        "org.kde.plasma.minimizeall",
        "org.kde.plasma.networkmanagement",
        "org.kde.plasma.notes",
        "org.kde.plasma.notifications",
        "org.kde.plasma.pager",
        "org.kde.plasma.panelspacer",
        "org.kde.plasma.printmanager",
        "org.kde.plasma.private.grouping",
        "org.kde.plasma.private.systemtray",
        "org.kde.plasma.quicklaunch",
        "org.kde.plasma.quickshare",
        "org.kde.plasma.remote",
        "org.kde.plasma.remote.digitalclock",
        "org.kde.plasma.showActivityManager",
        "org.kde.plasma.showdesktop",
        "org.kde.plasma.systemloadviewer",
        "org.kde.plasma.systemmonitor.cpu",
        "org.kde.plasma.systemmonitor.diskactivity",
        "org.kde.plasma.systemmonitor.diskusage",
        "org.kde.plasma.systemmonitor.memory",
        "org.kde.plasma.systemmonitor.net",
        "org.kde.plasma.systemtray",
        "org.kde.plasma.taskmanager",
        "org.kde.plasma.timer",
        "org.kde.plasma.trash",
        "org.kde.plasma.userswitcher",
        "org.kde.plasma.volume",
        "org.kde.plasma.weather",
        "org.kde.plasma.webbrowser",
        "org.kde.plasma.windowlist",
    };

    for(const QString &applet: applets) {
        new Plasma5AppletFactory(applet, &app);
    }

    qDebug() << "Registering service: org.plasma.provider.workspace";
    QDBusConnection::sessionBus().registerService("org.plasma.provider.workspace");
    app.exec();
}
