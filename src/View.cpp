#include "View.h"
#include "ControlPanel.h"
#include "DataBase.h"
#include "Gradient.h"
#include "IndepVar.h"
#include "Legend.h"
#include "Map.h"
#include "Settings.h"
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
#include<QtWidgets/qfiledialog.h>
#include<QtWidgets/qaction.h>
#include<QtWidgets/qmenubar.h>
// Grouping of object associations
class MapContext {
private:
  ControlPanel *cPanel = nullptr;
  Database *DB = nullptr;
  SQLiteTable *table = nullptr;
  Map *map = nullptr;
  std::map<unsigned int, unsigned int> cellNumToId;
  Legend *legend = nullptr;

public:
  void SetControlPanel(ControlPanel *_cPanel) { cPanel = _cPanel; }
  void SetDB(Database *_DB) { DB = _DB; }
  void SetTable(SQLiteTable *_table) { table = _table; };
  void SetMap(Map *_map) { map = _map; };
  void SetLegend(Legend *_legend) { legend = _legend; };
  void SetCellNumLookUp(const std::map<unsigned int, unsigned int> &lookup) {
    cellNumToId = lookup;
  }

  ControlPanel *GetControlPanel() const { return cPanel; };
  Database *GetDB() const { return DB; };
  SQLiteTable *GetTable() const { return table; };
  Map *GetMap() const { return map; };
  Legend *GetLegend() const { return legend; }
  std::map<unsigned int, unsigned int> &GetCellIdLookup() {
    return cellNumToId;
  };
};

MapContext mainContext;
View::View(QApplication *app) {
  Settings *settings = new Settings();
  settings->ReadSettings();
  mainContext.SetControlPanel(new ControlPanel(this));
  mainContext.SetDB(new Database());
  mainContext.SetMap(new Map());
  mainContext.SetLegend(new Legend(mainContext.GetMap()));
  mainWindow = new QWidget();
  QVBoxLayout *vLayout = new QVBoxLayout(mainWindow);

  vLayout->insertWidget(1, mainContext.GetControlPanel());
  vLayout->insertWidget(2, mainContext.GetMap(), 1);
  vLayout->addStretch();
  CreateMenuBar(mainWindow);

  mainWindow->show();
  }

std::string View::SelectFile() {
    QFileDialog dialog;
    QString QfileName = QFileDialog::getOpenFileName(nullptr, "", "");
    return QfileName.toStdString();
}
void View::CreateMenuBar(QWidget* parent)
{
        QMenuBar* menuBar = new QMenuBar(parent);
        QMenu* fileMenu = menuBar->addMenu("File");

        QAction* readAction = new QAction("Open", parent);
        fileMenu->addAction(readAction);
        QObject::connect(readAction, &QAction::triggered, [this]() { ReadDB(SelectFile()); });
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

  VorDiagram *vor = new VorDiagram();
mainContext.GetDB()->ProcessCells();
  std::vector<Cell *> vorCells = mainContext.GetDB()->GetVorCells();
  std::vector<Cell *> pathCells = mainContext.GetDB()->GetPathCells();
  std::vector<std::pair<double, double>> latlonPairs;
  for (const Cell *cell : vorCells) {
    latlonPairs.push_back({cell->lat, cell->lon});
  }
  vor->AddPoints(latlonPairs);
  vor->Generate();

  std::vector<Polygon *> polygons = vor->GetPolygons();
  std::vector<Cell *> cells;
  cells.insert(cells.end(),vorCells.begin(),vorCells.end());
  cells.insert(cells.end(),pathCells.begin(),pathCells.end());
  // Add in polygons for path cells
  for (const Cell *cell : pathCells) {
    const double gap = .5;
    Polygon *poly = new Polygon();

    poly->SetCenter({cell->lat,cell->lon});
    poly->AddCoord({cell->lat + gap, cell->lon - gap});
    poly->AddCoord({cell->lat + gap, cell->lon + gap});
    poly->AddCoord({cell->lat - gap, cell->lon + gap});
    poly->AddCoord({cell->lat - gap, cell->lon - gap});
    polygons.push_back(poly);
  }
  // Create a Marble QWidget without a parent
  for (const Polygon *poly : polygons) {

    unsigned int id = mainContext.GetMap()->AddPolygonMark(*poly);
    PolygonMark *mark = mainContext.GetMap()->GetPolygonMark(id);
    // mark->SetCoordinate(poly->GetCenter().x,poly->GetCenter().y);

    bool matchFound = false;
    unsigned int cellNum;
std::map<unsigned int, unsigned int> &cellNumToId =
      mainContext.GetCellIdLookup();
    for (size_t i = 0; i < cells.size(); i++) {
      double polyLat = poly->GetCenter().x;
      double polyLon = poly->GetCenter().y;
      Cell *currCell = cells[i];
      if (polyLat == currCell->lat && polyLon == currCell->lon) {
        cellNumToId[currCell->num] = id;
        std::string name = std::to_string(currCell->num);
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

}

void View::SimulationChanged(ControlPanel *cPanel) {
  Database *db = mainContext.GetDB();
  std::vector<IndepVar *> vars = cPanel->GetVars();

  for (const IndepVar *var : vars) {
    db->AddWhereClause(var->GetName(), var->GetSelection());
  }
  db->UpdateSimulation();
  cPanel->SetTimeOptions(db->GetTimeStepsStr());
  cPanel->SetResponses(db->GetRespScoreAsset());
}

void View::UpdateMap() {
  ControlPanel *cPanel = mainContext.GetControlPanel();
  Database *db = mainContext.GetDB();
  Map *map = mainContext.GetMap();
  map->SetPolygonColor({255, 255, 255, 0});
  double alpha = 125;
  // map->SetPolygonColor(QColor(255, 255, 255, alpha));
  const std::string respScoreAsset =
      cPanel->GetCurrentResponse() + "_" + cPanel->GetCurrentAsset();
  double time = cPanel->GetCurrentTime();
  const std::map<size_t, double> &cellSlice =
      db->GetCellSlice(respScoreAsset, time);

  double max = 0;

  for (const double &time : db->GetTimeSteps()) {
    const std::map<size_t, double> &slice =
        db->GetCellSlice(respScoreAsset, time);
    for (const std::pair<size_t, double> &cellData : slice) {
      double data = cellData.second;
      if (data > max) {
        max = data;
      }
    }
  }
  std::map<unsigned int, unsigned int> &cellNumToId =
      mainContext.GetCellIdLookup();

  Gradient *grad = new Gradient();
  // QColor floor(250, 36, 7, alpha);
  QColor floor(198, 255, 221, alpha);
  QColor mid(251, 215, 134, alpha);
  QColor ceil(247, 121, 125, alpha);
  grad->AddColorThresh(0, floor);
  grad->AddColorThresh(max / 2, mid);
  grad->AddColorThresh(max, ceil);
  Legend *leg = mainContext.GetLegend();
  leg->Clear();
  size_t numLeglevels = 4;
  for (size_t level = 0; level < numLeglevels; level++) {
    double thresh = max * level / numLeglevels;
    QColor color(grad->GetColor(thresh));
    leg->AddEntry(thresh, grad->GetColor(thresh));
  }
  for (const std::pair<size_t, double> &cellData : cellSlice) {
    size_t cell = cellData.first;
    double data = cellData.second;
    unsigned int id = cellNumToId[cell];
    if (data > 0) {
      map->SetPolygonColor(id, grad->GetColor(data));
    } else {
      map->SetPolygonColor(id, {255, 255, 255});
    }
  }
}
