#ifndef PARALLELOGRAM_H
#define PARALLELOGRAM_H

#include "polygon.h"

// Assigned to: Maurice Mejia

class Parallelogram: public Polygon {
public:
    Parallelogram(const Point& p1, const Point& p2, const Point& p3);
    /* Given three points, this will calculate the fourth point of the parallelogram.
       This fourth point forms a diagonal with p2.
    */
};

#endif // PARALLELOGRAM_H
