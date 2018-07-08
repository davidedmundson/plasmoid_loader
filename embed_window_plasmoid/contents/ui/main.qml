import QtQuick 2.0
import org.dave 1.0
import org.kde.plasma.plasmoid 2.0

EmbeddedProcess {
    id: root
    Plasmoid.icon: Plasmoid.configuration.icon
    Component.onCompleted: {
        //restart on change
        console.log("DAVE DAVE DAVE STARTING", Plasmoid.configuration.program)

        root.startProcess(Plasmoid.configuration.program, Plasmoid.configuration.args.split(" "));
    }
}
