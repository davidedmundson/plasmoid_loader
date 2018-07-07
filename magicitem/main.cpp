#include <QGuiApplication>
#include <QStringList>

#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include "main.h"
#include "embeddedwindow.h"
#include "embeddedprocessloader.h"

using namespace KWayland::Server;

void MagicItemPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<TestItem>(uri, 1, 0, "TestItem");
    qmlRegisterType<EmbeddedProcessLoader>(uri, 1, 0, "EmbeddedProcess");
}

#include "main.moc"
