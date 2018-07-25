#pragma once

#include <QObject>

/**
 *
 *
 *
 */

//communication over DBus/wayland should be in this class
//sourcing the data should be in the subclass
class AbstractAppletWrapper: public QObject
{
public:
    AbstractAppletWrapper(int id, QObject *parent);
    QString path() const;
private:
    const int m_id;
};
