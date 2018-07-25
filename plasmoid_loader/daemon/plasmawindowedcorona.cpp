/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 * Copyright 2014 Marco Martin <notmart@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "plasmawindowedcorona.h"
#include "plasmawindowedview.h"
#include <QDebug>
#include <QAction>
#include <QCommandLineParser>

#include <KLocalizedString>

#include <KPackage/Package>
#include <KPackage/PackageLoader>
#include <Plasma/PluginLoader>

PlasmaWindowedCorona::PlasmaWindowedCorona(QObject *parent)
    : Plasma::Corona(parent)
{
    KPackage::Package package = KPackage::PackageLoader::self()->loadPackage(QStringLiteral("Plasma/Shell"));
    package.setPath(QStringLiteral("org.kde.plasma.desktop"));
    setKPackage(package);
    load();
}

void PlasmaWindowedCorona::loadApplet(const QString &applet, const QVariantList &arguments)
{
    if (containments().isEmpty()) {
        return;
    }

    Plasma::Containment *cont = containments().first();
    PlasmaWindowedView *v = new PlasmaWindowedView();
    v->show();

    KConfigGroup appletsGroup(KSharedConfig::openConfig(), "Applets");
    QString plugin;
    for (const QString &group : appletsGroup.groupList()) {
        KConfigGroup cg(&appletsGroup, group);
        plugin = cg.readEntry("plugin", QString());

        if (plugin == applet) {
            Plasma::Applet *a = Plasma::PluginLoader::self()->loadApplet(applet, group.toInt(), arguments);
            if (!a) {
                qWarning() << "Unable to load applet" << applet << "with arguments" <<arguments;
                v->deleteLater();
                return;
            }
            a->restore(cg);

            //Access a->config() before adding to containment
            //will cause applets to be saved in palsmawindowedrc
            //so applets will only be created on demand
            KConfigGroup cg2 = a->config();
            cont->addApplet(a);

            v->setApplet(a);
            return;
        }
    }

    Plasma::Applet *a = Plasma::PluginLoader::self()->loadApplet(applet, 0, arguments);
    if (!a) {
        qWarning() << "Unable to load applet" << applet << "with arguments" <<arguments;
        v->deleteLater();
        return;
    }

    //Access a->config() before adding to containment
    //will cause applets to be saved in palsmawindowedrc
    //so applets will only be created on demand
    KConfigGroup cg2 = a->config();
    cont->addApplet(a);

    v->setApplet(a);
}

QRect PlasmaWindowedCorona::screenGeometry(int id) const
{
    Q_UNUSED(id);
    //TODO?
    return QRect();
}

void PlasmaWindowedCorona::load()
{
    /*this won't load applets, since applets are in plasmawindowedrc*/
    loadLayout(QStringLiteral("plasmawindowed-appletsrc"));


    bool found = false;
    for (auto c : containments()) {
        if (c->containmentType() == Plasma::Types::DesktopContainment) {
            found = true;
            break;
        }
    }

    if (!found) {
        qDebug() << "Loading default layout";
        createContainment(QStringLiteral("empty"));
        saveLayout(QStringLiteral("plasmawindowed-appletsrc"));
    }
}
