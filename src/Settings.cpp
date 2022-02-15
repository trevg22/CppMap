#include "Settings.h"
#include<marble/MarbleDirs.h>
#include<QString>
namespace varNames {
const std::string DataPath = "datapath";
const std::string pluginPath = "pluginpath";
} // namespace varNames
Settings::Settings() {}
void Settings::ReadSettings() {
  const std::string filename = "settings.lua";
  lua_State *state = luaL_newstate();
  luaL_dofile(state, filename.c_str());
  lua_getglobal(state, varNames::DataPath.c_str());
  if (!lua_isnil(state, -1)) {
    dataPath = lua_tostring(state, -1);
    Marble::MarbleDirs::setMarbleDataPath(QString::fromStdString(dataPath));
  }
  lua_close(state);
  
  localPath=Marble::MarbleDirs::localPath().toStdString();
  systemPath=Marble::MarbleDirs::systemPath().toStdString();
}

const std::string &Settings::GetDataPath() const { return dataPath; }

const std::string &Settings::GetLocalPath() const{return localPath;}

const std::string &Settings::GetSystemPath() const{return systemPath;}
