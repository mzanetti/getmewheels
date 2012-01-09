#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString location READ location WRITE setLocation)
public:
    explicit Settings(QObject *parent = 0);

    QString location();
    void setLocation(const QString &location);

private:
    QSettings *m_settings;
};

#endif // SETTINGS_H
