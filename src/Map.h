#pragma once
#include "Polygon.h"
#include "PolygonMark.h"
#include <map>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataLinearRing.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/MarbleWidget.h>
#include <qglobal.h>
#include <qobject.h>
#include"View.h"

class Map : public Marble::MarbleWidget {
  Q_OBJECT
private:
	View* view = nullptr;
  std::map<unsigned int, PolygonMark *> polyMarks;
  unsigned int renderCount = 0;
  Marble::GeoDataDocument *doc = nullptr;
  void MouseClicked(qreal lon, qreal lat);
  // void notifyMouseClick(int x, int y) ;
  //  void mousePressEvent(QMouseEvent *event);
  virtual ~Map(){};
  // void MouseClicked();

public:
  Map(View * _view);
  unsigned int AddPolygonMark(const Polygon &poly);
  PolygonMark *GetPolygonMark(unsigned int id);
  void SetPolygonColor(unsigned int id, const QColor &color);
  void SetPolygonColor(const QColor &color);
  void Finalize();
};
