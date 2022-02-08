#include "ControlPanel.h"

ControlPanel::ControlPanel() {
  hLayout = new QHBoxLayout(this);
  //timeSpin = new QDoubleSpinBox(this);
    timeSpin=new QComboBox();
  respDrop = new QComboBox(this);
  tgtDrop = new QComboBox(this);

  QVBoxLayout *timeLayout = new QVBoxLayout();
  QVBoxLayout *tgtLayout = new QVBoxLayout();
  QVBoxLayout *respLayout = new QVBoxLayout();
  QLabel *timeLabel = new QLabel("Time");
  QLabel *tgtLabel = new QLabel("Target");
  QLabel *respLabel = new QLabel("Response");
  timeLayout->insertWidget(0, timeLabel);
  timeLayout->insertWidget(1, timeSpin);
  tgtLayout->insertWidget(0, tgtLabel);
  tgtLayout->insertWidget(1, tgtDrop);
  respLayout->insertWidget(0, respLabel);
  respLayout->insertWidget(1, respDrop);
  hLayout->insertLayout(0, tgtLayout);
  hLayout->insertLayout(0, respLayout);
  hLayout->insertLayout(0, timeLayout);
  this->setLayout(hLayout);
}
void ControlPanel::AddIndepVar(IndepVar *var) {
  // hLayout->insertWidget(0, var->GetDrop());
  hLayout->insertLayout(0, var->GetVLayout());
}

void ControlPanel::SetTimeOptions(const std::vector<std::string> &timeOptions) {
  QList<QString> timeList;

  std::transform(timeOptions.begin(), timeOptions.end(),
                 std::back_inserter(timeList),
                 [](std::string str) { return QString::fromStdString(str); });
  timeSpin->addItems(timeList);
}
void ControlPanel::SetTimeParams(double start, double stop,
                                 unsigned int numSteps) {
  /*timeSpin->setMinimum(start);
  timeSpin->setMaximum(stop);
  timeSpin->setSingleStep((stop - start) / numSteps);
  */
}
void ControlPanel::SetResponses(
    const std::map<std::string, std::vector<std::string>> &_responsesByTgt) {
  responsesByTgt = _responsesByTgt;
}
