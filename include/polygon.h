#ifndef POLYGON_H
#define POLYGON_H

#include "point.h"

/* Assigned to: Apineh Asadoorian
    I decided to make one minor change to this class,
    but it shouldn't change the overall design.  We're
    not using a line class anymore, so the array here
    will store points instead.
*/

class Polygon {
public:
    Polygon(int size = 1);
    Polygon(const Polygon& other);
    Polygon& operator=(const Polygon& other);
    ~Polygon();
    Point Element(int i) const;
protected:
    Point& element(int i);
};

#endif // POLYGON_H
