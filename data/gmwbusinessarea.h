#ifndef GMWBUSINESSAREA_H
#define GMWBUSINESSAREA_H

#include <qgeocoordinate.h>
#include <QtCore/QList>

QTM_USE_NAMESPACE

class Area
{
public:
    Area() {}
    void setPath(QList<QGeoCoordinate> path) { m_path = path; }
    QList<QGeoCoordinate> path() const { return m_path; }

private:
    QList<QGeoCoordinate> m_path;
};

class GMWBusinessArea
{
public:
    GMWBusinessArea();

    QList<Area> areaList() const;
    void setAreaList(const QList<Area> &areaList);

    QList<Area> excludes() const;
    void setExcludes(const QList<Area> &excludes);

private:
    QList<Area> m_businessAreaList;
    QList<Area> m_excludes;

};

#endif // GMWBUSINESSAREA_H
