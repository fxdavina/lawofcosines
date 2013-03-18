#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"
#include <vector>

/* Assigned to: Apineh Asadoorian */

class Polygon {
public:
    Polygon(unsigned int size = 1);
    Polygon(const Polygon& other);
    Polygon& operator=(const Polygon& other);
    unsigned int Size() const;
    Point Element(unsigned int i) const;
    void Translate(double X, double Y);
    void copy(const Polygon &other);
protected:
    Point& element(unsigned int i);
private:
    std::vector<Point> pointsVector;
};

#endif // POLYGON_H
