#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "polygon.h"

//Assigned to: Ka Chan

class Triangle: public Polygon {
public:
    Triangle(const Point& p1, const Point& p2, const Point& p3);
    double Angle(int i); //This will return the angle opposite the side stored at index i
};

#endif // TRIANGLE_H
