#pragma once

#include "embeddedwindow.h"

class QProcess;

//this class is the publicly facable invocation of SurfaceItem
//it is responsible for launching a process, "catching it" and setting it as the surface

class EmbeddedProcessLoader: public SurfaceItem
{
    Q_OBJECT
public:
    EmbeddedProcessLoader(QQuickItem *parent = nullptr);
    Q_INVOKABLE void startProcess(const QString &exec, const QStringList &arguments);

private:
    QProcess *m_process = nullptr;
};
