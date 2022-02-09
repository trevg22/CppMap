#include "IndepVar.h"

IndepVar::IndepVar(const std::string &_name) {
  optionsDrop = new QComboBox();
  name = new QLabel(QString::fromStdString(_name));
  vLayout = new QVBoxLayout();
  vLayout->insertWidget(0, name);
  vLayout->insertWidget(1, optionsDrop);
}

std::string IndepVar::GetName() const { return name->text().toStdString(); }

std::string IndepVar::GetSelection() const {
  return optionsDrop->currentText().toStdString();
}
void IndepVar::SetOptions(const std::vector<std::string> &_options) {
  QList<QString> optionsList;
  std::transform(_options.begin(), _options.end(),
                 std::back_inserter(optionsList),
                 [](std::string str) { return QString::fromStdString(str); });

  optionsDrop->addItems(optionsList);
}
QComboBox *IndepVar::GetDrop() const { return optionsDrop; }

QVBoxLayout *IndepVar::GetVLayout() const { return vLayout; }
