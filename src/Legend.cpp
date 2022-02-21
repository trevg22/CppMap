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
  currColor = color;
  labels.push_back(std::to_string(value));
  unsigned int rectCornerx = colStart + colOffset;
  unsigned int rectCornery = rowStart + rowOffset * rowCount;
  double width = 20;
  double hieght = 20;

  //label->setGeometry(QRect(rectCornerx, rectCornery, width, hieght));

  QPainter painter;
}

void Legend::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  unsigned int rectCornerx = colStart + colOffset * 2;
  unsigned int rectCornery = rowStart;
  double width = 20;
  double hieght = 20;
  path.addRoundedRect(QRectF(rectCornerx, rectCornery, width, hieght),
                      width + 5, hieght + 5);
  QPen pen(Qt::black, 5);
  p.setPen(pen);
  p.fillPath(path, currColor);
  p.strokePath(path, pen);
  rowCount++;
  // p.drawPath(path);
}
