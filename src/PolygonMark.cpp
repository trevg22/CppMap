#include "PolygonMark.h"
#include <QtGui/qcolor.h>
#include <iostream>

PolygonMark::PolygonMark() {
  polyMark = new Marble::GeoDataPlacemark();
  style = new QSharedPointer<Marble::GeoDataStyle>(new Marble::GeoDataStyle);

  // mark->setStyle(*style);
  lineStyle = new Marble::GeoDataLineStyle();
  polyStyle = new Marble::GeoDataPolyStyle();
  polyStyle->setFill(true);
  style->data()->setLineStyle(*lineStyle);
  style->data()->setPolyStyle(*polyStyle);
}

bool PolygonMark::Contains(const coord &coord) {
  return ring->contains(
      {coord.x, coord.y, 0, Marble::GeoDataCoordinates::Radian});
}
void PolygonMark::SetGeometry(Marble::GeoDataLinearRing *_ring) {
  polyMark->setGeometry(_ring);
  ring = _ring;
}
void PolygonMark::SetCoordinate(double lat, double lon) {
  polyMark->setCoordinate(lon, lat, 0, Marble::GeoDataCoordinates::Degree);
}
Marble::GeoDataPlacemark *PolygonMark::GetPlaceMark() { return polyMark; }

void PolygonMark::SetName(const std::string &name) {
  polyMark->setName(QString::fromStdString(name));
}

void PolygonMark::SetLineColor(const QColor &color) {
  lineStyle->setColor(color);
}

void PolygonMark::SetLineWidth(double width )
{
    lineStyle->setWidth(width);
    style->data()->setLineStyle(*lineStyle);
}

void PolygonMark::SetPolygonColor(const QColor &color) {
  polyStyle->setColor(color);
  style->data()->setPolyStyle(*polyStyle);
}

void PolygonMark::SetCenterMark(Marble::GeoDataPlacemark *_centerMark) {
  centerMark = _centerMark;
}
Marble::GeoDataPolyStyle *PolygonMark::GetPolyStyle() { return polyStyle; }

Marble::GeoDataLineStyle *PolygonMark::GetLineStyle() { return lineStyle; }
QSharedPointer<Marble::GeoDataStyle> *PolygonMark::GetStyle() { return style; }
Marble::GeoDataPlacemark *PolygonMark::GetCenterMark() { return centerMark; }
