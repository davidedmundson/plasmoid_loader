import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.dave 1.0
import org.kde.plasma.plasmoid 2.0


RemotePlasmoid {
    Component.onCompleted: {
        setTargetPlugin("org.kde.plasma.taskmanager")
    }
    Layout.minimumWidth: minimumWidth
    Layout.minimumHeight: minimumHeight

    //this is always the view to show
    //we will then switch between compact/full on the remote side, potentially showing dialogs?
    Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation
}
