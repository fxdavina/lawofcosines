#ifndef SCREENPOLYGON_H
#define SCREENPOLYGON_H
#include "polygon.h"

class ScreenPolygon {
public:
    ScreenPolygon(Polygon* p);
    ScreenPolygon(const ScreenPolygon &other);
    ~ScreenPolygon();
    ScreenPolygon& operator=(const ScreenPolygon& other);
    void Draw() const;
    Polygon& Shape() const;
    double Scale;
    int Color;
};

#endif // SCREENPOLYGON_H
