#include "Gradient.h"

void Gradient::AddColorThresh(double thresh, const QColor &color) {
    colors.insert({thresh,color});
}

QColor Gradient::GetColor(double value) const{

    QColor *startColor=nullptr;
    QColor *stopColor=nullptr;
    double startValue=0;
    double stopValue=0;

    std::map<double,QColor>::const_iterator it=colors.lower_bound(value);
    std::map<double,QColor>::const_iterator prev=std::prev(it,2);
    stopColor=new QColor(it->second);
    startColor=new QColor(prev->second);
    startValue=prev->first;
    stopValue=it->first;
    //startColor=new QColor(--it->second);

    int redDelta=stopColor->red()-startColor->red();
    int greenDelta=stopColor->green()-startColor->green();
    int blueDelta=stopColor->blue()-startColor->blue();

    double ratio=(value-startValue)/stopValue-startValue;

    int red=startColor->red()+ratio*redDelta;
    int green=startColor->green()+ratio*greenDelta;
    int blue=startColor->blue()+ratio*blueDelta;
    return QColor(red,green,blue,255);
}
