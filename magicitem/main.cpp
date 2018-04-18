#include <QGuiApplication>
#include <QStringList>

#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include "main.h"
#include "embeddedwindow.h"

using namespace KWayland::Server;

void MagicItemPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<SurfaceItem>(uri, 1, 0, "MagicItem");
}

#include "main.moc"
