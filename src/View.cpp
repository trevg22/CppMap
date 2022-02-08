#include "View.h"
#include "ControlPanel.h"
#include "DataBase.h"
#include "IndepVar.h"
#include "MarbleMap.h"
#include "VorDiagram.h"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <SQLiteTable.h>
#include <iostream>
#include <map>
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataLineStyle.h>
#include <marble/GeoDataPolyStyle.h>
#include <marble/GeoDataStyle.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleModel.h>

// Grouping of object associations
class MapContext {
private:
  ControlPanel *cPanel = nullptr;
  Database *DB = nullptr;
  SQLiteTable *table = nullptr;
  MarbleMap *map = nullptr;

public:
  void SetControlPanel(ControlPanel *_cPanel) { cPanel = _cPanel; }
  void SetDB(Database *_DB) { DB = _DB; }
  void SetTable(SQLiteTable *_table) { table = _table; };
  void SetMap(MarbleMap *_map) { map = _map; };

  ControlPanel *GetControlPanel() const { return cPanel; };
  Database *GetDB() const { return DB; };
  SQLiteTable *GetTable() const { return table; };
  MarbleMap *GetMap() const { return map; };
};

MapContext mainContext;
View::View(QApplication *app) {
  mainContext.SetControlPanel(new ControlPanel());
  mainContext.SetDB(new Database());
  mainContext.SetMap(new MarbleMap());
  QWidget *widget = new QWidget();
  QVBoxLayout *vLayout = new QVBoxLayout(widget);
  vLayout->insertWidget(0, mainContext.GetControlPanel());
  vLayout->insertWidget(1, mainContext.GetMap(), 1);
  vLayout->addStretch();
  std::map<unsigned int, unsigned int> cellNumToId;

  VorDiagram *vor = new VorDiagram();
  ReadDB("map.db");
  mainContext.GetDB()->ProcessCells();
  std::vector<Cell *> DBCells = mainContext.GetDB()->GetCells();
  std::vector<std::pair<double, double>> latlonPairs;
  for (const Cell *cell : DBCells) {
    latlonPairs.push_back({cell->lat, cell->lon});
  }
  vor->AddPoints(latlonPairs);
  std::cout << "There are " << DBCells.size() << " cells\n";
  vor->Generate();
  // Create a Marble QWidget without a parent

  size_t count = vor->GetPolygons().size();
  for (const Polygon *poly : vor->GetPolygons()) {
    unsigned int id = mainContext.GetMap()->AddPolygonMark(*poly);
  }
  mainContext.GetMap()->Finalize();
  mainContext.GetMap()->resize(400, 300);
  mainContext.GetMap()->show();
  widget->show();
}

// This needs most optimization
void View::ReadDB(const std::string &fileName) {

  mainContext.GetDB()->SetName(fileName);
  mainContext.GetDB()->Open();
  std::map<std::string, std::vector<std::string>> varOptions;
  varOptions = mainContext.GetDB()->GetIndepVarOptions();

  ControlPanel *cPanel = mainContext.GetControlPanel();
  for (const std::pair<std::string, std::vector<std::string>> &ele :
       varOptions) {

    IndepVar *var = new IndepVar(ele.first);

    std::vector<std::string> options = ele.second;
    var->SetOptions(options);
    cPanel->AddIndepVar(var);
  }
  std::vector<std::string> timeOptions = mainContext.GetDB()->GetTimeSteps();
  cPanel->SetTimeOptions(timeOptions);
}
