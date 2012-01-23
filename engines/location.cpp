#include "location.h"

Location::Location()
{
}

int Location::id() const
{
    return m_id;
}

void Location::setId(int id)
{
    m_id = id;
}

QString Location::name() const
{
    return m_name;
}

void Location::setName(const QString &name)
{
    m_name = name;
}

QGeoBoundingBox Location::area() const
{
    return m_area;
}

void Location::setArea(const QGeoBoundingBox &area)
{
    m_area = area;
}
