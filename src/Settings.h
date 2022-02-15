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
  const std::string &GetDataPath() const;
  const std::string &GetLocalPath() const;
  const std::string &GetSystemPath() const;
};
