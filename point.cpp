#include "Point.h"
Point::Point(double x, double y)
{
    X = x;
    Y = y;
}
Point Point::Segment(double slope, double distance) {
    Point result;
	segment(X,Y,slope,distance,result.X,result.Y);
	return result;
}
double Point::Distance(const Point& p2) const {
    double x,
           y;
    x = p2.X;
    y = p2.Y;    
    return distance(X,Y,x,y);
}
double Point::Slope(const Point& p2) const {
	    double x,
           y;
    x = p2.X;
    y = p2.Y;
    return slope(X,Y,x,y);
}
double Point::Perpendicular(const Point& p2) const 
{
	return perpendicular(Slope(p2));
}
