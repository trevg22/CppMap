#include "DataBase.h"

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
std::vector<std::string> Database::GetTimeSteps() {
  SQLiteTable mavData;
  mavData.SetName(SQLKey::dataTable);
  UpdateTableSchema(mavData);
  return GetDistinctColumnValues(mavData, SQLKey::time);
}

std::vector<std::string> Database::GetRespScoreAsset() {
  SQLiteTable mavData;
  mavData.SetName(SQLKey::dataTable);
  UpdateTableSchema(mavData);
  return GetDistinctColumnValues(mavData, SQLKey::reponse);
}
