#pragma once
#include "Polygon.h"
#include <marble/GeoDataLineStyle.h>
#include <marble/GeoDataLinearRing.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoDataPolyStyle.h>
#include <marble/GeoDataStyle.h>
class PolygonMark {
private:
  Marble::GeoDataPlacemark *polyMark = nullptr;
  Marble::GeoDataPlacemark *centerMark = nullptr;
  Marble::GeoDataLinearRing *poly = nullptr;
  QSharedPointer<Marble::GeoDataStyle> *style = nullptr;
  Marble::GeoDataLineStyle *lineStyle = nullptr;
  Marble::GeoDataPolyStyle *polyStyle = nullptr;
  Marble::GeoDataLinearRing *ring = nullptr;

public:
  PolygonMark();
  bool Contains(const coord &coor);
  void SetCoordinate(double lat, double on);
  void SetGeometry(Marble::GeoDataLinearRing *_ring);
  void SetName(const std::string &name);
  void SetLineColor(const QColor &color);
  void SetLineWidth(double width);
  void SetPolygonColor(const QColor &color);
  void SetCenterMark(Marble::GeoDataPlacemark *_centerMark);
  Marble::GeoDataPlacemark *GetPlaceMark();
  Marble::GeoDataPolyStyle *GetPolyStyle();
  Marble::GeoDataLineStyle *GetLineStyle();
  QSharedPointer<Marble::GeoDataStyle> *GetStyle();
  Marble::GeoDataPlacemark *GetCenterMark();
};
