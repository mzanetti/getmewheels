#ifndef CORE_H
#define CORE_H

#include "data/gmwitemmodel.h"
#include "data/gmwitemsortfilterproxymodel.h"
#include "engines/gmwengine.h"

#include <QObject>
#include <QGeoServiceProvider>

class Core : public QObject
{
    Q_OBJECT
public:
    static Core* instance();

    QGeoServiceProvider *serviceProvider();

private:
    explicit Core(QObject *parent = 0);
    static Core *s_instance;

    QGeoServiceProvider *m_serviceProvider;

};

#endif // CORE_H
