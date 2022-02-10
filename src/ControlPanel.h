#include "IndepVar.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qwidget.h>
#include <map>
#include <string>
#include <vector>
#include"View.h"
class ControlPanel : public QWidget {
private:
  View *view=nullptr;
  QHBoxLayout *hLayout = nullptr;
  QComboBox *timeSpin = nullptr;
  QComboBox *assetDrop = nullptr;
  QComboBox *respDrop = nullptr;
  // Valid responses for a target key
  std::map<std::string, std::vector<std::string>> assetsByResponse;
  std::vector<IndepVar *> vars;
  void ResponseSelected(const std::string &response);
  void SimulationChanged();

public:
  ControlPanel(View *_view);
  void ConnectSignals();
  void AddIndepVar(IndepVar *var);
  void SetTimeOptions(const std::vector<std::string> &timeOptions);
  void SetTimeParams(double start, double stop, unsigned int numSteps);
  void SetResponses(const std::vector<std::string> &respScoreAssets);
  std::vector<IndepVar*> GetVars();
  std::string GetCurrentAsset();
  std::string GetCurrentResponse();
  double GetCurrentTime();
};
