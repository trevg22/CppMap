#include "PolygonMark.h"
#include <QtGui/qcolor.h>

PolygonMark::PolygonMark() {
  mark = new Marble::GeoDataPlacemark();
  style = new QSharedPointer<Marble::GeoDataStyle>(new Marble::GeoDataStyle);

  lineStyle = new Marble::GeoDataLineStyle(QColor(255,255,0,70));
  polyStyle = new Marble::GeoDataPolyStyle(QColor(255,255,255,70));
  polyStyle->setFill(true);
  style->data()->setLineStyle(*lineStyle);
  style->data()->setPolyStyle(*polyStyle);
  mark->setStyle(*style);
}

void PolygonMark::SetGeometry(Marble::GeoDataLinearRing *ring) {
  mark->setGeometry(ring);
}
void PolygonMark::SetCoordinate(double lat, double lon) {
  mark->setCoordinate(lat, lon, 0, Marble::GeoDataCoordinates::Degree);
}
Marble::GeoDataPlacemark *PolygonMark::GetPlaceMark() { return mark; }
