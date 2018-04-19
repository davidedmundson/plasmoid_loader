#include "embeddedprocessloader.h"

#include <QProcess>
#include <QProcessEnvironment>
#include <KWayland/Server/shell_interface.h>

EmbeddedProcessLoader::EmbeddedProcessLoader(QQuickItem *parent):
    SurfaceItem(parent)
{
}

void EmbeddedProcessLoader::startProcess(const QString &exec, const QStringList &args)
{
    if (m_process) {
        m_process->kill();
        m_process->deleteLater();
    }
    m_process = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("WAYLAND_DISPLAY", "plasma0");
    env.insert("QT_QPA_PLATFORM", "wayland");
    env.insert("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1");
    env.insert("SDL_VIDEODRIVER", "wayland");
    m_process->setProcessEnvironment(env);
    m_process->setProgram(exec);
    m_process->setArguments(args);

    m_process->start();

    //FUTURE, watch process lifespan, maybe restart ?

    connect(Compositor::self(), &Compositor::newSurface, this, [=](KWayland::Server::ShellSurfaceInterface *ssi) {
        //TODO if PID blah blah
        // if (!surface);
        setSurface(ssi);
    });

    qDebug() << "DAVE, started " << m_process->pid();
}
