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
    Polygon(unsigned int size = 1);
    Polygon(const Polygon& other);
    Polygon& operator=(const Polygon& other);
    ~Polygon();
    unsigned int Size() const;
    Point Element(unsigned int i) const;
protected:
    Point& element(unsigned int i);
};

#endif // POLYGON_H
