#include "Triangle.h"

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3):Polygon(3)
{
	element(0) = p1;
	element(1) = p2;
	element(2) = p3;
}

double Triangle::Angle(int i)
{
	int p1 = (i + 1) % 3; //can get one of other point
	int p2 = (i + 2) % 3; //can get one of other point
	double a = getLength(p1); //get one of length of triangle
	double b = getLength(p2); //get one of length of triangle
	double c = getLength(i);  //get the length opposite the given point
	return angle(a,b,c);	  //return the angle from given point
}

double Triangle::getLength(int point)
{
	int p1 = (point + 1) % 3; //can get one of other point
	int p2 = (point + 2) % 3; //can get one of other point
	return element(p1).distance(element(p2)); //return the length opposite point
}
