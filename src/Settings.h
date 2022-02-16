#include "lua.hpp"
#include <string>
class Settings {
private:
  std::string dataPath;
  std::string localPath;
  std::string systemPath;

public:
  Settings();
  void ReadSettings();
  std::string GetDataPath() const;
  std::string GetLocalPath() const;
  std::string GetSystemPath() const;
};
