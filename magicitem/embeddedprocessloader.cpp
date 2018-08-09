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
    env.insert("IRR_DEVICE_TYPE", "wayland");
    m_process->setProcessEnvironment(env);
    m_process->setProgram(exec);
    m_process->setArguments(args);
    m_process->setProcessChannelMode(QProcess::ForwardedChannels);
    m_process->start();

    //FUTURE, watch process lifespan, maybe restart ?

    connect(Compositor::self(), &Compositor::newSurface, this, [=](KWayland::Server::XdgShellSurfaceInterface *toplevel) {
        if (surface()) {
//             return;
        }
        qDebug() << "new surface";
        if (toplevel->client()->processId() != m_process->pid()) {
            qDebug() << m_process->pid() << toplevel->client()->processId();
//             return;
        }
        setSurface(toplevel->surface());
        toplevel->configure(0, size().toSize());

        connect(this, &SurfaceItem::sizeChanged, toplevel, [=]() {
            toplevel->configure(0, size().toSize());
        });


        Compositor::self()->registerContainer(this, toplevel->surface());
    });


    qDebug() << "DAVE, started " << m_process->pid();
}

EmbeddedProcessLoader::~EmbeddedProcessLoader()
{
    if (m_process) {
        m_process->kill();
    }
}
