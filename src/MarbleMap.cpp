#include "MarbleMap.h"
MarbleMap::MarbleMap() {
  // Load the OpenStreetMap map
  this->setMapThemeId("earth/bluemarble/bluemarble.dgml");

  this->setProjection(Marble::Mercator);

  // Enable the cloud cover and enable the country borders
  // this->setShowClouds(true);
  //this->setShowBorders(true);
  this->setShowGrid(false);

  // Hide the FloatItems: Compass and StatusBar
}

unsigned int MarbleMap::AddPlaceMark() {
  marks.emplace(renderCount, new Marble::GeoDataPlacemark());
  renderCount++;
  return renderCount - 1;
}

Marble::GeoDataPlacemark * MarbleMap::GetPlaceMark(unsigned int renderId)
{
    return marks[renderId];
}


