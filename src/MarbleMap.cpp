#include "MarbleMap.h"
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleModel.h>
MarbleMap::MarbleMap() {
  // Load the OpenStreetMap map
  this->setMapThemeId("earth/bluemarble/bluemarble.dgml");

  this->setProjection(Marble::Mercator);

  // Enable the cloud cover and enable the country borders
  // this->setShowClouds(true);
  // this->setShowBorders(true);
  this->setShowGrid(false);
  doc = new Marble::GeoDataDocument();
  // Hide the FloatItems: Compass and StatusBar
}

unsigned int MarbleMap::AddPolygonMark(const Polygon &poly) {
  Marble::GeoDataLinearRing *ring = new Marble::GeoDataLinearRing();
  for (const coord &coord : poly.GetCoords()) {
    *ring << Marble::GeoDataCoordinates(coord.y, coord.x, 0,
                                        Marble::GeoDataCoordinates::Degree);
  }
  PolygonMark *mark = new PolygonMark();
  const coord &center = poly.GetCenter();
  mark->SetCoordinate(center.x, center.y);
  mark->SetName("blah");
  mark->SetGeometry(ring);
  // mark->GetPlaceMark()->setStyle(style);
  polyMarks.insert(
      std::map<unsigned int, PolygonMark *>::value_type(renderCount, mark));
  doc->append(mark->GetPlaceMark());
  renderCount++;
  return renderCount - 1;
}
PolygonMark *MarbleMap::GetPolygonMark(unsigned int id) {
  return polyMarks.at(id);
}

void MarbleMap::Finalize() { this->model()->treeModel()->addDocument(doc); }
