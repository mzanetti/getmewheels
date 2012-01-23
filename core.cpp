#include "core.h"
#include "engines/car2go/car2goengine.h"

#include <QDir>

Core *Core::s_instance = 0;
Core *Core::instance()
{
    if(!s_instance) {
        s_instance = new Core();
    }
    return s_instance;
}

Core::Core(QObject *parent) :
    QObject(parent)
{
#ifndef QT_SIMULATOR
    m_serviceProvider = new QGeoServiceProvider("openstreetmap");
#else
    m_serviceProvider = new QGeoServiceProvider("nokia");
#endif
}

QGeoServiceProvider *Core::serviceProvider()
{
    return m_serviceProvider;
}
