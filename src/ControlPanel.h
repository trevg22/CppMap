#include "IndepVar.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qwidget.h>
#include <map>
#include <string>
#include <vector>
class ControlPanel : public QWidget {
private:
  QHBoxLayout *hLayout = nullptr;
  QComboBox *timeSpin = nullptr;
  QComboBox *tgtDrop = nullptr;
  QComboBox *respDrop = nullptr;
  // Valid responses for a target key
  std::map<std::string, std::vector<std::string>> responsesByTgt;
  std::vector<IndepVar *> vars;

public:
  ControlPanel();
  void AddIndepVar(IndepVar *var);
  void ResponseSelected();
  void SetTimeOptions(const std::vector<std::string> &timeOptions);
  void SetTimeParams(double start, double stop, unsigned int numSteps);
  void SetResponses(
      const std::map<std::string, std::vector<std::string>> &_responsesByTgt);
};
