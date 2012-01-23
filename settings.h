#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include "engines/location.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    void writeDefaultLocation(const Location *location);
    bool readDefaultLocation(Location *location);

private:
    QSettings *m_settings;
};

#endif // SETTINGS_H
