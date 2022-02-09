#include "ControlPanel.h"
#include <iostream>


ControlPanel::ControlPanel(View *_view) {
  view=_view;
  hLayout = new QHBoxLayout(this);
  // timeSpin = new QDoubleSpinBox(this);
  timeSpin = new QComboBox();
  respDrop = new QComboBox(this);
  assetDrop = new QComboBox(this);

  QVBoxLayout *timeLayout = new QVBoxLayout();
  QVBoxLayout *tgtLayout = new QVBoxLayout();
  QVBoxLayout *respLayout = new QVBoxLayout();
  QLabel *timeLabel = new QLabel("Time");
  QLabel *tgtLabel = new QLabel("Asset");
  QLabel *respLabel = new QLabel("Response");
  timeLayout->insertWidget(0, timeLabel);
  timeLayout->insertWidget(1, timeSpin);
  tgtLayout->insertWidget(0, tgtLabel);
  tgtLayout->insertWidget(1, assetDrop);
  respLayout->insertWidget(0, respLabel);
  respLayout->insertWidget(1, respDrop);
  hLayout->insertLayout(0, tgtLayout);
  hLayout->insertLayout(0, respLayout);
  hLayout->insertLayout(0, timeLayout);
  this->setLayout(hLayout);

  connect(
      respDrop, &QComboBox::currentTextChanged, this,
      [this](const QString &text) { ResponseSelected(text.toStdString()); });
}

void ControlPanel::ResponseSelected(const std::string &response) {
  std::vector<std::string> &assetVec = assetsByResponse[response];
  QList<QString> assetList;

  std::transform(assetVec.begin(), assetVec.end(),
                 std::back_inserter(assetList),
                 [](std::string str) { return QString::fromStdString(str); });
  assetDrop->clear();
  assetDrop->addItems(assetList);
}

void ControlPanel::AddIndepVar(IndepVar *var) {
  hLayout->insertLayout(0, var->GetVLayout());
  connect(var->GetDrop(), &QComboBox::currentTextChanged, this,
          [this]() { view->SimulationChanged(this); });
  vars.push_back(var);

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
void ControlPanel::SetResponses(const std::vector<std::string> &respScoreAssets) {
for (const std::string &str : respScoreAssets) {
    size_t underPos = str.find("_");
    if (underPos != std::string::npos) {
      std::string response = str.substr(0, underPos);
      std::string asset = str.substr(underPos+1, str.length());
      assetsByResponse[response].push_back(asset);
    }
  }
  std::map<std::string, std::vector<std::string>>::iterator first =
      assetsByResponse.begin();

  std::vector<std::string> respNames;
  for (const std::pair<std::string, std::vector<std::string>> &ele :
       assetsByResponse) {
    respNames.push_back(ele.first);
  }

  QList<QString> respList;

  std::transform(respNames.begin(), respNames.end(),
                 std::back_inserter(respList),
                 [](std::string str) { return QString::fromStdString(str); });

  respDrop->addItems(respList);
}

std::vector<IndepVar*> ControlPanel::GetVars()
{
return vars;
}

