#pragma once
#include<QtWidgets/qwidget.h>
#include<QColor>
#include<QtWidgets/qboxlayout.h>
#include <qlabel.h>
#include<vector>
#include<QPainterPath>

class Legend:public QWidget{
    private:
    struct entry{
        QColor* color=nullptr;
        QLabel* label=nullptr;
        QPainterPath *path=nullptr;
    };
    QVBoxLayout *vLayout;
    const unsigned int rowStart=10;
    const unsigned int colStart=0;
    const unsigned int rowOffset=40;
    const unsigned int colOffset=50;
    std::vector<entry *> entries;
    

    public:
    Legend(QWidget* parent);
    void AddEntry(double value,const QColor&color);
    void paintEvent(QPaintEvent *);
    void Paint();
    void Clear();
};
