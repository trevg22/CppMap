#pragma once
#include <marble/GeoDataLineStyle.h>
#include <marble/GeoDataLinearRing.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoDataPolyStyle.h>
#include <marble/GeoDataStyle.h>
class PolygonMark {
private:
  Marble::GeoDataPlacemark *mark = nullptr;
  Marble::GeoDataLinearRing *poly = nullptr;
  QSharedPointer<Marble::GeoDataStyle> *style = nullptr;
  Marble::GeoDataLineStyle *lineStyle = nullptr;
  Marble::GeoDataPolyStyle *polyStyle = nullptr;

public:
  PolygonMark();
  void SetCoordinate(double lat, double on);
  void SetGeometry(Marble::GeoDataLinearRing *ring);
  void SetName(const std::string &name);
  void SetLineColor(const QColor &color);
  void SetPolygonColor(const QColor &color);
  Marble::GeoDataPlacemark *GetPlaceMark();
};
