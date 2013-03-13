#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>

double distance(double x1, double y1, double x2, double y2)
/* Written by: Dylan La Com
Returns the distance between two points.
*/

double slope(double x1, double y1, double x2, double y2)
/* Written by: Shang Yi (Sam) Huang
Calculates the slope between two points.
*/


double perpendicular(double slope)
/*
Written by: Davina Doedijono
Returns a slope perpendicular to the given slope
*/

double angle(double a, double b, double c)
/*
Written by: Davina Boedijono
Given a triangle with side lengths a,b and c, returns
the value of the angle opposite side c in degrees.
*/


void segment(double x1, double y1, double slope, double distance, double& x2, double& y2)
/*
Written by: Pui Yee Lee
Calculates the coordinates the point which forms the line segment at the specified slope
and distance from (x1,y1).  A positive distance gives x2 > x1, and negative distance
gives x2 < x1
*/




#endif // GEOMETRY_H
