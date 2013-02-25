#ifndef SCREENPOLYGON_H
#define SCREENPOLYGON_H
#include "polygon.h"

typedef void (*DRAWFUNC)(ScreenPolygon*);

class ScreenPolygon {
public:
    ScreenPolygon(Polygon* p);
    ScreenPolygon(const ScreenPolygon &other);
    ~ScreenPolygon();
    ScreenPolygon& operator=(const ScreenPolygon& other);
    void Draw();
    Polygon Shape() const;
    DRAWFUNC DrawFunction;
    double Scale;
    int Color;
protected:
    Polygon* shape();
};

#endif // SCREENPOLYGON_H
