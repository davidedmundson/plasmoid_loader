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

#include "plasmawindowedview.h"

#include <QQuickItem>
#include <QResizeEvent>
#include <QQmlExpression>
#include <QMenu>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlProperty>

#include <KLocalizedString>

#include <Plasma/Package>


PlasmaWindowedView::PlasmaWindowedView(QWindow *parent)
    : QQuickView(parent),
      m_applet(nullptr)
{
    engine()->rootContext()->setContextProperty(QStringLiteral("root"), contentItem());

    //DAVE - WTF is this?
    //access appletInterface.Layout.minimumWidth, to create the Layout attached object for appletInterface as a sideeffect
    QQmlExpression *expr = new QQmlExpression(engine()->rootContext(), contentItem(),
                                              QStringLiteral("Qt.createQmlObject('import QtQuick 2.0; import QtQuick.Layouts 1.1; import org.kde.plasma.core 2.0; Item {anchors.fill:parent; property Item appletInterface; onAppletInterfaceChanged: print(appletInterface.Layout.minimumWidth)}', root, \"\");"));
    m_rootObject = expr->evaluate().value<QQuickItem*>();
    setColor(QColor(Qt::transparent));
    setResizeMode(QQuickView::SizeRootObjectToView);
}

PlasmaWindowedView::~PlasmaWindowedView()
{
}

void PlasmaWindowedView::setApplet(Plasma::Applet *applet)
{
    m_applet = applet;
    if (!applet) {
        return;
    }

    m_appletInterface = applet->property("_plasma_graphicObject").value<QQuickItem *>();

    if (!m_appletInterface) {
        return;
    }

    m_appletInterface->setParentItem(m_rootObject);
    m_rootObject->setProperty("appletInterface", QVariant::fromValue(m_appletInterface.data()));
    m_appletInterface->setVisible(true);
    setTitle(applet->title());
    setIcon(QIcon::fromTheme(applet->icon()));

    const QSize switchSize(m_appletInterface->property("switchWidth").toInt(), m_appletInterface->property("switchHeight").toInt());

    foreach (QObject *child, m_appletInterface->children()) {
        //find for the needed property of Layout: minimum/maximum/preferred sizes and fillWidth/fillHeight
        if (child->property("minimumWidth").isValid() && child->property("minimumHeight").isValid() &&
                child->property("preferredWidth").isValid() && child->property("preferredHeight").isValid() &&
                child->property("maximumWidth").isValid() && child->property("maximumHeight").isValid() &&
                child->property("fillWidth").isValid() && child->property("fillHeight").isValid()
           ) {
            m_layout = child;
        }
    }

    if (m_layout) {
        connect(m_layout, SIGNAL(minimumWidthChanged()), this, SLOT(minimumWidthChanged()));
        connect(m_layout, SIGNAL(minimumHeightChanged()), this, SLOT(minimumHeightChanged()));
    }
    qDebug() << "layout!!??" << m_layout;

    minimumWidthChanged();
    minimumHeightChanged();


    m_appletInterface->setSize(size());
    contentItem()->setSize(size());
}

void PlasmaWindowedView::resizeEvent(QResizeEvent *ev)
{
    if (!m_applet || !m_appletInterface) {
        return;
    }

    m_appletInterface->setSize(ev->size());
    contentItem()->setSize(ev->size());
}


//void PlasmaWindowedView::hideEvent(QHideEvent *ev)
//{
//    Q_UNUSED(ev)
//    m_applet->config().sync();
//    if (!m_withStatusNotifier) {
//        m_applet->deleteLater();
//        deleteLater();
//    }
//}

//void PlasmaWindowedView::showConfigurationInterface(Plasma::Applet *applet)
//{
//    if (m_configView) {
//        m_configView->hide();
//        m_configView->deleteLater();
//    }

//    if (!applet || !applet->containment()) {
//        return;
//    }

//    m_configView = new PlasmaQuick::ConfigView(applet);

//    m_configView->init();
//    m_configView->show();
//}

void PlasmaWindowedView::minimumWidthChanged()
{
    if (!m_layout || !m_appletInterface) {
        return;
    }
    qDebug() << "minSize" << m_layout->property("minimumWidth").toInt();


    setMinimumWidth(m_layout->property("minimumWidth").toInt());
}

void PlasmaWindowedView::minimumHeightChanged()
{
    if (!m_layout || !m_appletInterface) {
        return;
    }
    setMinimumHeight(m_layout->property("minimumHeight").toInt());
}

void PlasmaWindowedView::maximumWidthChanged()
{
    if (!m_layout) {
        return;
    }

    setMaximumWidth(m_layout->property("maximumWidth").toInt());
}

void PlasmaWindowedView::maximumHeightChanged()
{
    if (!m_layout) {
        return;
    }

    setMaximumHeight(m_layout->property("maximumHeight").toInt());
}

