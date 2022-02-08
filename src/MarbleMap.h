#pragma once
#include <marble/MarbleWidget.h>
#include<marble/GeoDataPlacemark.h>
#include<marble/GeoDataLinearRing.h>
#include"Polygon.h"
#include<map>
#include"PolygonMark.h"
#include<marble/GeoDataDocument.h>

class MarbleMap : public Marble::MarbleWidget {
    private:
    std::map<unsigned int,PolygonMark *> polyMarks;
    unsigned int renderCount=0;
    Marble::GeoDataDocument *doc=nullptr;
    public:
    MarbleMap();
    unsigned int AddPolygonMark(const Polygon &poly);
    void Finalize();
};
