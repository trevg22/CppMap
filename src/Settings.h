#include <string>
#include"lua.hpp"
class Settings {
private:
  std::string dataPath;
  std::string localPath; std::string systemPath;

public:
  Settings();
  void ReadSettings();
  const std::string& GetDataPath ()const;
};
