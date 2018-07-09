/*
 *  Copyright 2013 David Edmundson <davidedmundson@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
 */

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import org.kde.kquickcontrolsaddons 2.0 as KQuickAddons

GridLayout {
    property alias cfg_program: programInput.text
    property alias cfg_args: argumentsInput.text

    columns: 2
    Label {
        text: i18n("Program")
    }
    TextField {
        id: programInput
    }
    Label {
        text: i18n("Arguments")
    }
    TextField {
        id: argumentsInput
    }
    Item {
        Layout.fillHeight: true
    }

    Label {
        text: i18n("Icon")
    }
//             Button {
//                 id: iconButton
//                 Layout.minimumWidth: previewFrame.width + units.smallSpacing * 2
//                 Layout.maximumWidth: Layout.minimumWidth
//                 Layout.minimumHeight: previewFrame.height + units.smallSpacing * 2
//                 Layout.maximumHeight: Layout.minimumWidth
//
//                 DragDrop.DropArea {
//                     id: dropArea
//
//                     property bool containsAcceptableDrag: false
//
//                     anchors.fill: parent
//
//                     onDragEnter: {
//                         // Cannot use string operations (e.g. indexOf()) on "url" basic type.
//                         var urlString = event.mimeData.url.toString();
//
//                         // This list is also hardcoded in KIconDialog.
//                         var extensions = [".png", ".xpm", ".svg", ".svgz"];
//                         containsAcceptableDrag = urlString.indexOf("file:///") === 0 && extensions.some(function (extension) {
//                             return urlString.indexOf(extension) === urlString.length - extension.length; // "endsWith"
//                         });
//
//                         if (!containsAcceptableDrag) {
//                             event.ignore();
//                         }
//                     }
//                     onDragLeave: containsAcceptableDrag = false
//
//                     onDrop: {
//                         if (containsAcceptableDrag) {
//                             // Strip file:// prefix, we already verified in onDragEnter that we have only local URLs.
//                             iconDialog.setCustomButtonImage(event.mimeData.url.toString().substr("file://".length));
//                         }
//                         containsAcceptableDrag = false;
//                     }
//                 }
//
//                 KQuickAddons.IconDialog {
//                     id: iconDialog
//
//                     function setCustomButtonImage(image) {
//                         cfg_customButtonImage = image || cfg_icon || "start-here-kde"
//                         cfg_useCustomButtonImage = true;
//                     }
//
//                     onIconNameChanged: setCustomButtonImage(iconName);
//                 }
//
//                 // just to provide some visual feedback, cannot have checked without checkable enabled
//                 checkable: true
//                 checked: dropArea.containsAcceptableDrag
//                 onClicked: {
//                     checked = Qt.binding(function() { // never actually allow it being checked
//                         return iconMenu.status === PlasmaComponents.DialogStatus.Open || dropArea.containsAcceptableDrag;
//                     })
//
//                     iconMenu.open(0, height)
//                 }
//
//                 PlasmaCore.FrameSvgItem {
//                     id: previewFrame
//                     anchors.centerIn: parent
//                     imagePath: plasmoid.location === PlasmaCore.Types.Vertical || plasmoid.location === PlasmaCore.Types.Horizontal
//                             ? "widgets/panel-background" : "widgets/background"
//                     width: units.iconSizes.large + fixedMargins.left + fixedMargins.right
//                     height: units.iconSizes.large + fixedMargins.top + fixedMargins.bottom
//
//                     PlasmaCore.IconItem {
//                         anchors.centerIn: parent
//                         width: units.iconSizes.large
//                         height: width
//                         source: cfg_useCustomButtonImage ? cfg_customButtonImage : cfg_icon
//                     }
//                 }
//             }

}
