#include "gmwbusinessarea.h"

#include <QDebug>

GMWBusinessArea::GMWBusinessArea()
{
}

void GMWBusinessArea::setAreaList(const QList<Area> &areaList)
{
    qDebug() << "arealist has" << areaList.count() << "entries";
    m_businessAreaList = areaList;
}

QList<Area> GMWBusinessArea::areaList() const
{
    return m_businessAreaList;
}

void GMWBusinessArea::setExcludes(const QList<Area> &excludes)
{
    m_excludes = excludes;
}

QList<Area> GMWBusinessArea::excludes() const
{
    return m_excludes;
}
