#pragma once
#include <QtWidgets/qapplication.h>
#include <string>
class ControlPanel;
class View {
private:
public:
  View(QApplication *app);
  void ReadDB(const std::string &fileName);
  void UpateMap(ControlPanel *cPanel);
};
