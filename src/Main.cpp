#include "MarbleMap.h"
#include "VorDiagram.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <map>
#include <marble/AbstractFloatItem.h>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataLineStyle.h>
#include <marble/GeoDataPolyStyle.h>
#include <marble/GeoDataStyle.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleGlobal.h>
#include <marble/MarbleModel.h>
#include <marble/MarbleWidget.h>
#include<QtWidgets/qwidget.h>
#include<QtWidgets/qboxlayout.h>
#include<QtWidgets/qpushbutton.h>

QSharedPointer<Marble::GeoDataStyle> *style = nullptr;
void toggleColor() {
  static bool red = true;
std::cout<<"Called\n";
  if (red) {
    style->data()->polyStyle().setColor(QColor(0, 0, 255));
    red=false;
  } else {
    style->data()->polyStyle().setColor(QColor(255, 0, 0));
    red=true;
  }
}
int main(int argc, char **argv) {
  QApplication app(argc, argv);
  std::map<unsigned int, unsigned int> cellNumToId;

  VorDiagram *vor = new VorDiagram();
  vor->ReadCSV("points2");
  vor->Generate();
  // Create a Marble QWidget without a parent
  QWidget * widget=new QWidget();
  QVBoxLayout *vLayout=new QVBoxLayout(widget);
  MarbleMap *mapWidget = new MarbleMap();
  vLayout->addWidget(mapWidget);
  QPushButton * button=new QPushButton(widget);
  QObject::connect(button,&QAbstractButton::pressed,toggleColor);
  Marble::GeoDataDocument *doc = new Marble::GeoDataDocument();
  // Marble::GeoDataStyle *style = new Marble::GeoDataStyle;
  style = new QSharedPointer<Marble::GeoDataStyle>(new Marble::GeoDataStyle);
  Marble::GeoDataLineStyle lineStyle(QColor(255, 255, 0, 90));
  lineStyle.setWidth(8);

  Marble::GeoDataPolyStyle *polyStyle =
      new Marble::GeoDataPolyStyle(QColor(255, 0, 0, 40));
  polyStyle->setFill(true);

  style->data()->setLineStyle(lineStyle);
  style->data()->setPolyStyle(*polyStyle);
  size_t count = vor->GetPolygons().size();
  for (const Polygon *poly : vor->GetPolygons()) {
    unsigned int id = mapWidget->AddPlaceMark();
    Marble::GeoDataLinearRing *ring = MarbleMap::PolygonToLinearRing(*poly);
    // Marble::GeoDataLinearRing* ring= new Marble::GeoDataLinearRing()
    Marble::GeoDataPlacemark *mark = mapWidget->GetPlaceMark(id);
    coord center = poly->GetCenter();
    mark->setCoordinate(center.y, center.x, 0,
                        Marble::GeoDataCoordinates::Degree);
    mark->setName("Heeellllo");
    mark->setGeometry(ring);

    mark->setStyle(*style);
    doc->append(mark);
    // std::cout<<ring->isClosed()<<"\n";
  }

  unsigned int id = mapWidget->AddPlaceMark();
  Marble::GeoDataPlacemark *mark = mapWidget->GetPlaceMark(id);
  double lon = 26;
  double lat = 44;
  Marble::GeoDataLinearRing ring2;
  ring2 << Marble::GeoDataCoordinates(lon + 5, lat + 5, 0,
                                      Marble::GeoDataCoordinates::Degree)
        << Marble::GeoDataCoordinates(lon - 5, lat + 5, 0,
                                      Marble::GeoDataCoordinates::Degree)
        << Marble::GeoDataCoordinates(lon + 5, lat - 5, 0,
                                      Marble::GeoDataCoordinates::Degree)
        << Marble::GeoDataCoordinates(lon - 5, lat - 5, 0,
                                      Marble::GeoDataCoordinates::Degree);

  mark->setCoordinate(lat, lon, 0, Marble::GeoDataCoordinates::Degree);
  mark->setName("Blaaaah");
  mark->setGeometry(&ring2);
  doc->append(mark);
  mapWidget->model()->treeModel()->addDocument(doc);
  mapWidget->resize(400, 300);
//mapWidget->show();
  widget->show();

  return app.exec();
}
