#ifndef SCREENPOLYGON_H
#define SCREENPOLYGON_H
#include <allegro.h>
#include "polygon.h"

class ScreenPolygon {
public:
    ScreenPolygon(Polygon* p);
    ScreenPolygon(const ScreenPolygon &other);
    ~ScreenPolygon();
    ScreenPolygon& operator=(const ScreenPolygon& other);
    void Draw(BITMAP* buffer) const;
    Polygon& Shape() const;
    void Translate(double X, double Y);
    double Scale;
    int Color;
private:
    Polygon* poly;
};

#endif // SCREENPOLYGON_H
