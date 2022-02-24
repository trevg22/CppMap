#include <SQLiteDB.h>
#include <string>
#include <vector>

class Simulation {
private:
  // data by response,time,cell
  std::map<std::string, std::map<double, std::map<size_t,double>>> data;

public:
  const std::map<std::string, std::map<double, std::map<size_t,double>>> &
  GetData() {
    return data;
  }

  void AddElement(const std::string &response, double time,
                  size_t cell, double value) {
    data[response][time][cell] = value;
  }
  double GetElement(const std::string &response, double time,
                    size_t cell, double value) {
    return data[response][time][cell];
  }
  std::map<std::size_t,double>&GetCellSlice(const std::string &response,double time)
  {
      return data[response][time];
  }
  void Clear()
  {
      data.clear();
  }
};

struct Cell {
  int num;
  double lat;
  double lon;
  bool isPath;
};
class CellTable : public SQLiteTable {
public:
  void Init() {
    InsertEle("Cell", SQL::INT);
    InsertEle("Lat", SQL::REAL);
    InsertEle("Lon", SQL::REAL);
  }
};
namespace SQLKey {
const std::string cellTable = "Cells";
const std::string dataTable = "Data";
const std::string cell = "Cell";
const std::string lat = "Lat";
const std::string lon = "Lon";
const std::string pathCell = "PathCell";
const std::string reponse = "Response";
const std::string time = "Time";
const std::string value = "Val";
} // namespace SQLKey

class Database : public SQLiteDB {
private:
  std::vector<Cell *> vorCells;
  std::vector<Cell*> pathCells;
  Simulation *currSim = nullptr;
  std::vector<double> timeSteps;
  std::vector<std::string> respScoreAssets;

public:
  Database();
  void ProcessCells();
  void UpdateSimulation();
  std::vector<Cell *> GetVorCells();
  std::vector<Cell*> GetPathCells();
  std::map<std::string, std::vector<std::string>> GetIndepVarOptions();
  std::vector<std::string> GetTimeStepsStr();
  std::vector<double> GetTimeSteps();
  std::vector<std::string> GetRespScoreAsset();
std::map<std::size_t,double>&GetCellSlice(const std::string &response,double time);
};
