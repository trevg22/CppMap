#include<SQLiteDB.h>
#include<string>
#include<vector>
struct Cell{
    int num;
    double lat;
    double lon;
    bool isPath;
};
class CellTable:public SQLiteTable
{
    public:
    void Init()
    {
        InsertEle("Cell",SQL::INT);
        InsertEle("Lat",SQL::REAL);
        InsertEle("Lon",SQL::REAL);
    }
};
namespace SQLKey{
   const std::string cellTable="Cells";
    const std::string dataTable="Data";
    const std::string cell="Cell";
    const std::string lat="Lat";
    const std::string lon="Lon";
    const std::string pathCell="PathCell";
    const std::string reponse="Response";
    const std::string time="Time";
    const std::string value="Val";
}

class Database:public SQLiteDB{
    private:
    std::vector<Cell*> cells;
    public:
    void ProcessCells();
    std::vector<Cell*> GetCells();
    std::map<std::string,std::vector<std::string>> GetIndepVarOptions();
    std::vector<std::string> GetTimeSteps();
    std::vector<std::string> GetRespScoreAsset();
};
