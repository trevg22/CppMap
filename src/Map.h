#pragma once
#include "Polygon.h"
#include "PolygonMark.h"
#include <map>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataLinearRing.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/MarbleWidget.h>
#include<marble/MarbleWidgetInputHandler.h>
#include<qobject.h>

class Map : public Marble::MarbleWidget {
  Q_OBJECT
private:
  std::map<unsigned int, PolygonMark *> polyMarks;
  unsigned int renderCount = 0;
  Marble::GeoDataDocument *doc = nullptr;
  Marble::MarbleWidgetInputHandler *handler = nullptr;
  //void notifyMouseClick(int x, int y) ;
  // void mousePressEvent(QMouseEvent *event);
  virtual ~Map(){};
  // void MouseClicked(qreal lon, qreal lat, Marble::GeoDataCoordinates unit);
  void MouseClicked();

public:
  Map();
  unsigned int AddPolygonMark(const Polygon &poly);
  PolygonMark *GetPolygonMark(unsigned int id);
  void SetPolygonColor(unsigned int id, const QColor &color);
  void SetPolygonColor(const QColor &color);
  void Finalize();
};
