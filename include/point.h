#ifndef POINT_H
#define POINT_H

#include "geometry.h"

/* Assigned to: Jacky Yang

I decided to do something slightly different.  We're basically
going to merge the Point and Line classes because it's somewhat
more efficient, and we don't necessarily need to deal with
Lines as discrete objects.

*/

struct Point {
    Point(double x = 0, double y = 0);
    Point Segment(double slope, double distance);
    double Distance(const Point& p2) const;
    double Slope(const Point& p2) const;
    double Perpendicular(const Point& p2) const;

    double X;
    double Y;
};

#endif // POINT_H
