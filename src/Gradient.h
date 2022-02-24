#include<QColor>
#include<map>
class Gradient{
    private:
        std::map<double,QColor> colors;
    public:
    void AddColorThresh(double thresh,const QColor &color);
    QColor GetColor(double value) const;
};
