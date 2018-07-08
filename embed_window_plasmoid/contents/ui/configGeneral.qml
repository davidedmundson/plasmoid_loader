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

GridLayout {
    property alias cfg_program: programInput.text
    property alias cfg_args: argumentsInput.text
//     property alias cfg_icon:

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

}
