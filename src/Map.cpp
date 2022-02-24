#include "Map.h"
#include <iostream>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleModel.h>
using namespace Marble;

Map::Map() {
  // Load the OpenStreetMap map
  this->setMapThemeId("earth/bluemarble/bluemarble.dgml");
  this->setProjection(Marble::Mercator);
  this->setShowScaleBar(false);
  this->setShowCompass(false);
  this->setShowGrid(false);
  doc = new Marble::GeoDataDocument();
  Marble::MarbleWidget *parent = dynamic_cast<MarbleWidget *>(this);
  // this->connect(this,SIGNAL(mouseClickGeoPosition(qreal,qreal,Marble::GeoDataCoordinates::Unit)),this,SLOT(MouseClicked()));
  // this->connect(nullptr,SIGNAL(Marble::mouseClickGeoPosition(qreal,qreal,Marble::GeoDataCoordinates::Unit)),this,SLOT(MouseClicked()));
  this->connect(this, &MarbleWidget::mouseClickGeoPosition, this,
                &Map::MouseClicked);
  this->connect(this, &MarbleWidget::mouseClickGeoPosition, this,
                [this](qreal lon, qreal lat) { MouseClicked(lon, lat); });
}
// void Map::MouseClicked()
void Map::MouseClicked(qreal lon, qreal lat) {
  std::cout << "Mouse clicked" << lon << " " << lat << "\n";
  double latRad = lat * 180 / std::numbers::pi;
  double lonRad = lon * 180 / std::numbers::pi;
  for (const std::pair<unsigned int, PolygonMark *> ele : polyMarks) {
    if (ele.second->Contains({lonRad, latRad})) {
      float width = ele.second->GetLineStyle()->width();
      ele.second->SetLineWidth(2 * width);
      //std::cout << "Doubled width\n";
      std::cout <<"Clicked on cell with name "<<ele.second->GetCenterMark()->displayName().toStdString()<<"\n";

      this->model()->treeModel()->updateFeature(ele.second->GetPlaceMark());
    }
  }
  // double radLat=lat*180/math.pi;
}

// void Map::notifyMouseClick(int x,int y)
// {
//     std::cout <<"Please Work";
// }
// void MarbleMap::mouseClickGeoPosition

unsigned int Map::AddPolygonMark(const Polygon &poly) {
  Marble::GeoDataLinearRing *ring = new Marble::GeoDataLinearRing();
  for (const coord &coord : poly.GetCoords()) {
    *ring << Marble::GeoDataCoordinates(coord.y, coord.x, 0,
                                        Marble::GeoDataCoordinates::Degree);
  }
  static int callCount = 0;
  callCount++;
  PolygonMark *mark = new PolygonMark();
  const coord &center = poly.GetCenter();
  mark->GetPlaceMark()->setStyle(*mark->GetStyle());
  mark->GetPlaceMark()->setVisible(true);
  mark->SetGeometry(ring);
  // mark->GetPlaceMark()->setCoordinate(center.y,center.x,0,Marble::GeoDataCoordinates::Degree);
  Marble::GeoDataPlacemark *centerMark = new Marble::GeoDataPlacemark();
  centerMark->setCoordinate(center.y, center.x, 0,
                            Marble::GeoDataCoordinates::Degree);
  // centerMark->setName("blah");
  polyMarks.insert(
      std::map<unsigned int, PolygonMark *>::value_type(renderCount, mark));
  // mark->SetPolygonColor({255,0,0,30});
  // mark->GetPolyStyle()->setFill(false);
  mark->SetCenterMark(centerMark);
  mark->GetPlaceMark()->setBalloonVisible(true);
  doc->append(mark->GetPlaceMark());
  doc->append(centerMark);
  // this->model()->treeModel()->addFeature(doc,mark->GetPlaceMark());
  renderCount++;
  return renderCount - 1;
}
PolygonMark *Map::GetPolygonMark(unsigned int id) { return polyMarks.at(id); }
void Map::SetPolygonColor(unsigned int id, const QColor &color) {
  polyMarks[id]->SetPolygonColor(color);
  // this->doc->append(polyMarks[id]->GetPlaceMark());
  //  this->model()->treeModel()->addDocument(doc);
  this->model()->treeModel()->updateFeature(polyMarks[id]->GetPlaceMark());
}
void Map::SetPolygonColor(const QColor &color) {

  for (const std::pair<unsigned int, PolygonMark *> ele : polyMarks) {
    // std::cout<<"\n"<<"Setting color for id "<<ele.first<<"\n";
    PolygonMark *mark = ele.second;
    mark->SetPolygonColor(color);
    this->model()->treeModel()->updateFeature(mark->GetPlaceMark());
  }
}
void Map::Finalize() { this->model()->treeModel()->addDocument(doc); }
