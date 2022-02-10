#include "DataBase.h"
#include <iostream>
Database::Database() { currSim = new Simulation(); }
void Database::ProcessCells() {
  CellTable table;
  table.Init();
  std::vector<std::string> columnNames = {SQLKey::cell, SQLKey::lat,
                                          SQLKey::lon, SQLKey::pathCell};
  table.SetName(SQLKey::cellTable);
  std::string SQLSelect = BuildSelect(table, columnNames);
  SQLSelect += BuildWhere(table);
  SQLSelect += ";";
  // std::cout << "SQL Where " + SQLSelect;
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, SQLSelect.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    printf("error:%s ", sqlite3_errmsg(db));
  }

  int res = SQLITE_OK;
  while (res != SQLITE_DONE) {
    res = sqlite3_step(stmt);
    Cell *cell = new Cell();
    switch (res) {
    case SQLITE_ROW:
      cell->num = sqlite3_column_int(stmt, 0);
      cell->lat = sqlite3_column_int(stmt, 1);
      cell->lon = sqlite3_column_double(stmt, 2);
      cell->isPath = sqlite3_column_int(stmt, 3);
      if (cell->isPath) {
      } else {
        cells.push_back(cell);
      }
      break;
    case SQLITE_DONE:
      break;
    }
  }
  // extract this to function
  sqlite3_finalize(stmt);
}

void Database::UpdateSimulation() {
  currSim->Clear();

  SQLiteTable mavData;
  mavData.SetName(SQLKey::dataTable);
  UpdateTableSchema(mavData);
  std::vector<std::string> columnNames = {SQLKey::cell, SQLKey::reponse,
                                          SQLKey::time, SQLKey::value};
  timeSteps = GetDistinctColumnValues(mavData, SQLKey::time);

  respScoreAssets = GetDistinctColumnValues(mavData, SQLKey::reponse);

  std::string SQLCMD = BuildSelect(mavData, columnNames) + BuildWhere(mavData);
  SQLCMD += ";";
  sqlite3_stmt *stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, SQLCMD.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    printf("error:%s ", sqlite3_errmsg(db));
  }

  size_t cellCol = mavData.GetColumNum(SQLKey::cell);
  size_t respCol = mavData.GetColumNum(SQLKey::reponse);
  size_t timeCol = mavData.GetColumNum(SQLKey::time);
  size_t valCol = mavData.GetColumNum(SQLKey::value);
  int res = SQLITE_OK;
  while (res != SQLITE_DONE) {
    res = sqlite3_step(stmt);
    switch (res) {
    case SQLITE_ROW: {
      int cell = sqlite3_column_int(stmt, 0);
      std::string response(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
      double time=sqlite3_column_double(stmt,2);
      double val = sqlite3_column_double(stmt, 3);
      currSim->AddElement(response, time, cell, val);
      break;
    }
    case SQLITE_DONE: {
      break;
    }
    }
  }
  // extract this to function
}
std::vector<Cell *> Database::GetCells() { return cells; }

std::map<std::string, std::vector<std::string>> Database::GetIndepVarOptions() {
  SQLiteTable mavData;
  std::map<std::string, std::vector<std::string>> options;
  mavData.SetName(SQLKey::dataTable);
  UpdateTableSchema(mavData);

  const std::map<std::string, DBColumn> &eleMap = mavData.GetTable();
  for (const std::pair<std::string, DBColumn> &col : eleMap) {
    const std::string colName = col.first;
    if (colName != SQLKey::time && colName != SQLKey::reponse &&
        colName != SQLKey::value && colName != SQLKey::cell) {
      options[colName] = GetDistinctColumnValues(mavData, colName);
    }
  }
  return options;
}
std::vector<std::string> Database::GetTimeSteps() { return timeSteps; }

std::vector<std::string> Database::GetRespScoreAsset() {
  return respScoreAssets;
}
std::map<std::size_t,double>& Database::GetCellSlice(const std::string &response,double time)
{
    return currSim->GetCellSlice(response,time);
}
