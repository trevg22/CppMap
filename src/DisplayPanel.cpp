#include "DisplayPanel.h"

DisplayPanel::DisplayPanel() {
  rows = new QVBoxLayout();
  this->setLayout(rows);
}
void DisplayPanel::AddDataEntry(const std::string &label, double data) {
  respEntries.push_back(
      {new QLabel(QString::fromStdString(label)),
       new QLabel(QString::fromStdString(std::to_string(data)))});

  QHBoxLayout *col = new QHBoxLayout();
  col->insertWidget(0, respEntries.end()->label);
  col->insertWidget(1,respEntries.end()->value);
  rows->addLayout(col);
}

void DisplayPanel::Clear() { 
    respEntries.clear();
    rows.cle
}
