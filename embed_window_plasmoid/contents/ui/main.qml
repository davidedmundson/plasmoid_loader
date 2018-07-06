import QtQuick 2.0
import org.dave 1.0

EmbeddedProcess {
    id: root
    Component.onCompleted: {
        console.log("HERE");
        root.startProcess("dolphin");
    }
}
