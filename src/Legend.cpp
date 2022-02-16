#include "Legend.h"
#include <QtGui/qpainter.h>
#include <qlabel.h>
#include <qstring.h>
Legend::Legend(QWidget *parent) : QWidget(parent) {
  vLayout = new QVBoxLayout();
  this->setGeometry(100, 100, 100, 100);
  this->setLayout(vLayout);
  this->show();
}
void Legend::AddEntry(double value, const QColor &color) {
  QPainter painter;
  QHBoxLayout *hLayout = new QHBoxLayout();
  QLabel *label = new QLabel(QString::fromStdString(std::to_string(value)));
  hLayout->insertWidget(0, label);
  vLayout->addLayout(hLayout);
}

void Legend::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setPen(Qt::blue);
  painter.setFont(QFont("Arial", 30));
  painter.drawText(rect(), Qt::AlignCenter, "Qt");
}
