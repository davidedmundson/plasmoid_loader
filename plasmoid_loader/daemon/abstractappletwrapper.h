#pragma once

#include <QObject>

class AbstractAppletWrapper: public QObject
{
public:
    AbstractAppletWrapper(int id, QObject *parent);
    QString path() const;
private:
    const int m_id;
};
