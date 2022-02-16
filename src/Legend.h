#pragma once
#include<QtWidgets/qwidget.h>
#include<QColor>
#include<QtWidgets/qboxlayout.h>
class Legend:public QWidget{
    private:
    QVBoxLayout *vLayout;
    public:
    Legend(QWidget* parent);
    void AddEntry(double value,const QColor&color);
    void paintEvent(QPaintEvent *);
};
