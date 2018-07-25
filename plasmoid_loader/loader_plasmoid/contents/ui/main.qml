import QtQuick 2.0
import org.dave 1.0

RemotePlasmoid {
    Component.onCompleted: {
        setTargetPlugin("org.kde.plasma.digitalclock")
    }
}
