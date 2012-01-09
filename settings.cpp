#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    m_settings = new QSettings("getmewheels");

}

QString Settings::location()
{
    return m_settings->value("location").toString();
}

void Settings::setLocation(const QString &location)
{
    m_settings->setValue("location", location);
}
