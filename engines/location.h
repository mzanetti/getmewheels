#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QString>
#include <QGeoBoundingBox>

QTM_USE_NAMESPACE

class Location: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
public:
    Location();

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QGeoBoundingBox area() const;
    void setArea(const QGeoBoundingBox &area);
private:
    int m_id;
    QString m_name;
    QGeoBoundingBox m_area;

};

#endif // LOCATION_H
