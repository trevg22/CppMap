#pragma once
#include <QtWidgets/qapplication.h>
#include <string>
class ControlPanel;
class View {
private:
	QWidget* mainWindow = nullptr;
  void CreateMenuBar(QWidget* parent);
  std::string SelectFile();
public:
  View(QApplication *app);
  void ReadDB(const std::string &fileName);
  void UpdateMap();
  void SimulationChanged(ControlPanel *cPanel);
};
