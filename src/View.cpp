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
  std::map<unsigned int, unsigned int> cellNumToId;

public:
  void SetControlPanel(ControlPanel *_cPanel) { cPanel = _cPanel; }
  void SetDB(Database *_DB) { DB = _DB; }
  void SetTable(SQLiteTable *_table) { table = _table; };
  void SetMap(MarbleMap *_map) { map = _map; };
  void SetCellNumLookUp(const std::map<unsigned int, unsigned int> &lookup) {
    cellNumToId = lookup;
  }

  ControlPanel *GetControlPanel() const { return cPanel; };
  Database *GetDB() const { return DB; };
  SQLiteTable *GetTable() const { return table; };
  MarbleMap *GetMap() const { return map; };
  std::map<unsigned int, unsigned int> &GetCellIdLookup() {
    return cellNumToId;
  };
};

MapContext mainContext;
View::View(QApplication *app) {
  mainContext.SetControlPanel(new ControlPanel(this));
  mainContext.SetDB(new Database());
  mainContext.SetMap(new MarbleMap());
  QWidget *widget = new QWidget();
  QVBoxLayout *vLayout = new QVBoxLayout(widget);
  vLayout->insertWidget(0, mainContext.GetControlPanel());
  vLayout->insertWidget(1, mainContext.GetMap(), 1);
  vLayout->addStretch();
  std::map<unsigned int, unsigned int> &cellNumToId =
      mainContext.GetCellIdLookup();

  VorDiagram *vor = new VorDiagram();
  ReadDB("map2.db");
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
    PolygonMark *mark = mainContext.GetMap()->GetPolygonMark(id);
    // mark->SetCoordinate(poly->GetCenter().x,poly->GetCenter().y);

    bool matchFound = false;
    unsigned int cellNum;
    for (size_t i = 0; i < DBCells.size(); i++) {
      double polyLat = poly->GetCenter().x;
      double polyLon = poly->GetCenter().y;
      Cell *currCell = DBCells[i];
      if (polyLat == currCell->lat && polyLon == currCell->lon) {
         cellNumToId[currCell->num] = id;
        std::string name = std::to_string(i);
        mark->GetCenterMark()->setName(QString::fromStdString(name));

        matchFound = true;
      }
    }
    if (!matchFound) {
      std::cout << "Warning no match for polygon found\n";
    }
  }
  SimulationChanged(mainContext.GetControlPanel());
  mainContext.GetControlPanel()->ConnectSignals();
  mainContext.GetMap()->Finalize();
  UpdateMap();
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
}

void View::SimulationChanged(ControlPanel *cPanel) {
  Database *db = mainContext.GetDB();
  std::vector<IndepVar *> vars = cPanel->GetVars();

  for (const IndepVar *var : vars) {
    db->AddWhereClause(var->GetName(), var->GetSelection());
  }
  db->UpdateSimulation();
  cPanel->SetTimeOptions(db->GetTimeSteps());
  cPanel->SetResponses(db->GetRespScoreAsset());
}

void View::UpdateMap() {
    std::cout<<"Map updated";
  ControlPanel *cPanel = mainContext.GetControlPanel();
  Database *db = mainContext.GetDB();
  MarbleMap *map = mainContext.GetMap();
  double alpha=125;
  map->SetPolygonColor(QColor(255,255,255,alpha));
  const std::string respScoreAsset =
      cPanel->GetCurrentResponse() + "_" + cPanel->GetCurrentAsset();
  std::cout<<respScoreAsset<<"\n";
  double time = cPanel->GetCurrentTime();
  const std::map<size_t, double> &cellSlice =
      db->GetCellSlice(respScoreAsset, time);

  double max = 0;
  for (const std::pair<size_t, double> &cellData : cellSlice) {
    double data = cellData.second;
    if (data > max) {
      max = data;
    }
  }
  std::map<unsigned int, unsigned int> &cellNumToId =
      mainContext.GetCellIdLookup();
  for (const std::pair<size_t, double> &cellData : cellSlice) {
    size_t cell = cellData.first;
    double data = cellData.second;
    unsigned int id=cellNumToId[cell];
    std::cout<<"Color for cell "<<cell<< "set to "<<255*data/max<<"for id "<<id<<" \n";
    QColor color(255*data/max,0,0,255);
    map->SetPolygonColor(id,color);
  }
}
