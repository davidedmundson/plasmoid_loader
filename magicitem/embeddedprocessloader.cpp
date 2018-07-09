#include "embeddedprocessloader.h"

#include <QProcess>
#include <QProcessEnvironment>
#include <KWayland/Server/xdgshell_interface.h>
#include <KWayland/Server/clientconnection.h>

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

    connect(Compositor::self(), &Compositor::newSurface, this, [=](KWayland::Server::XdgShellSurfaceInterface *ssi) {
        //TODO if PID blah blah
        if (ssi->client()->processId() != m_process->pid()) {
            qDebug() << m_process->pid() << ssi->client()->processId();
            return;
        }
        setSurface(ssi->surface());
        Compositor::self()->registerContainer(this, ssi->surface());
    });

    qDebug() << "DAVE, started " << m_process->pid();
}

EmbeddedProcessLoader::~EmbeddedProcessLoader()
{
    if (m_process) {
        m_process->kill();
    }
}
