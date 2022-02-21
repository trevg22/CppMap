#include "Legend.h"
#include <QtGui/qpainter.h>
#include <QtGui/qpainterpath.h>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <iostream>
#include <qlabel.h>
#include <qstring.h>

Legend::Legend(QWidget *parent) : QWidget(parent) {
  vLayout = new QVBoxLayout();
  this->setGeometry(0, 130, 300, 300);
  QGraphicsOpacityEffect op(this);
  op.setOpacity(.5);
  this->setGraphicsEffect(&op);
  // this->setAutoFillBackground(true) //  this->setLayout(vLayout);
  this->show();
}
void Legend::AddEntry(double value, const QColor &color) {
  entry *ent = new entry();
  ent->label = new QLabel(QString::fromStdString(std::to_string(value)), this);
  ent->color = new QColor(color);
  entries.push_back(ent);
}

void Legend::Paint() {}
void Legend::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  const double labelWidth = 30;
  const double labelHieght = 30;
  const double colorWidth=30;
  const double colorHieght=30;
  std::cout << "Painted " << entries.size() << "\n";
  for (size_t row = 0; row < entries.size(); row++) {
    unsigned int labelCornerx = colStart + colOffset;
    unsigned int labelCornery = rowStart + rowOffset * row;
    QLabel *label = entries[row]->label;

    label->setGeometry(QRect(labelCornerx, labelCornery, labelWidth, labelHieght));
    label->show();

    unsigned int colorCornerx = colStart + colOffset * 2;
    path.addRoundedRect(QRectF(colorCornerx, labelCornery, colorWidth, colorHieght),
                        colorWidth -20 , colorHieght -20);
    QPen pen(Qt::black, 5);
    p.setPen(pen);
    p.fillPath(path, *entries[row]->color);
    p.strokePath(path, pen);
  }
  // p.drawPath(path);
}
