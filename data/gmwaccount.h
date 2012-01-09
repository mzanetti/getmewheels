#ifndef GMWACCOUNT_H
#define GMWACCOUNT_H

#include <QString>
#include <QMetaType>

class GMWAccount
{
public:
    GMWAccount(int id = -1, const QString &decription = QString());

    bool isValid() const;

    int id() const;
    QString description() const;

    bool operator==(const GMWAccount &other) const;
    bool operator!=(const GMWAccount &other) const;

private:
    int m_id;
    QString m_description;
};
Q_DECLARE_METATYPE(GMWAccount)
#endif // GMWACCOUNT_H
