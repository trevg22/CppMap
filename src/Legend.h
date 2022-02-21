#pragma once
#include<QtWidgets/qwidget.h>
#include<QColor>
#include<QtWidgets/qboxlayout.h>
#include<vector>
class Legend:public QWidget{
    private:
    QVBoxLayout *vLayout;
    QColor currColor;
    const unsigned int rowStart=10;
    const unsigned int colStart=0;
    const unsigned int rowOffset=20;
    const unsigned int colOffset=40;
    unsigned int rowCount=0;
    std::vector<QColor*> colors;
    std::vector<std::string> labels;
    

    public:
    Legend(QWidget* parent);
    void AddEntry(double value,const QColor&color);
    void paintEvent(QPaintEvent *);
};
