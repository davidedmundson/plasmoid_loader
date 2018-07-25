import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.dave 1.0

RemotePlasmoid {
    Component.onCompleted: {
        setTargetPlugin("org.kde.plasma.digitalclock")
    }
    Layout.minimumWidth: minimumWidth
    Layout.minimumHeight: minimumHeight

}
