#include "abstractappletwrapper.h"
#include "appletadaptor.h"

#include <QDebug>

AbstractAppletWrapper::AbstractAppletWrapper(int id, QObject *parent):
    QObject(parent),
    m_id(id)
{
    new AppletAdaptor(this);

    QDBusConnection::sessionBus().registerObject(path(), this);
    qDebug() << "constructed " << path();
}

QString AbstractAppletWrapper::path() const
{
    return QStringLiteral("/Plasma/") + QString::number(m_id);
}
