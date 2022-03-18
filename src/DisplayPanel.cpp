#include "DisplayPanel.h"
#include<QtWidgets/qcolordialog.h>

DisplayPanel::DisplayPanel(QWidget *parent) :QWidget(parent){
  rows = new QVBoxLayout();
  this->setLayout(rows);
  this->setGeometry(0, 300, 300, 300);
  this->show();
}
void DisplayPanel::AddDataEntry(const std::string &label, double data) {
    QColor color = QColorDialog::getColor(Qt::black, this);
    Entry* ent = new Entry;
    ent->label = new QLabel(QString::fromStdString(label));
    ent->value=new QLabel(QString::fromStdString(std::to_string(data)));
    QPalette palette = ent->label->palette();
    palette.setColor(QPalette::WindowText, color);
    
    ent->label->setPalette(palette);
    ent->value->setPalette(palette);
    respEntries.push_back(ent);

  QHBoxLayout *col = new QHBoxLayout();
  col->insertWidget(0, ent->label);
  col->insertWidget(1, ent->value);
  rows->addLayout(col);
}

void clearLayout(QLayout* layout) {
    if (layout == NULL)
        return;
    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}
void DisplayPanel::Clear() { 
    for (Entry *ent : respEntries)
    {
        if (ent)
        {
            if (ent->label)
            {
                delete ent->label;
            }

            if (ent->value)
            {

                delete ent->value;
            }
            delete ent;
        }
    }
    respEntries.clear();
}
