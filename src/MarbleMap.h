#include <marble/MarbleWidget.h>
#include<marble/GeoDataPlacemark.h>
#include<marble/GeoDataLinearRing.h>
#include"Polygon.h"
#include<map>

class MarbleMap : public Marble::MarbleWidget {
    private:
    std::map<unsigned int,Marble::GeoDataPlacemark*> marks;
    unsigned int renderCount=0;
    public:
    MarbleMap();
    unsigned int AddPlaceMark();
    Marble::GeoDataPlacemark * GetPlaceMark(unsigned int renderId);
    static Marble::GeoDataLinearRing* PolygonToLinearRing(const Polygon & poly)
    {
        Marble::GeoDataLinearRing *ring=new Marble::GeoDataLinearRing();

        for(const coord & coord:poly.GetCoords())
        {
            //ring->append({coord.y,coord.x,0,Marble::GeoDataCoordinates::Degree});
            *ring<<Marble::GeoDataCoordinates(coord.y,coord.x,0,Marble::GeoDataCoordinates::Degree);
        }
        return ring;
    }

};
