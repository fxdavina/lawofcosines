#include "Point.h"
Point::Point(double X, double Y)
{
    x=X;
    y=Y;
}
Point Segment(double slope, double distance) {
    Point result;
	segment(X,Y,slope,distance,result.X,result.Y);
	return result;
}
double Distance(const Point& p2) const {
    return distance(X,Y,p2.X,p2.Y);
}
double Slope(const Point& p2) const {
	return slope(X,Y,p2.X,p2.Y);
}
double Perpendicular(const Point& p2) const [
	return perpendicular(Slope(p2));
}