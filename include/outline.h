#ifndef OUTLINE_H
#define OUTLINE_H

#include "polygon.h"

class Outline: public Polygon {
public:
    Outline(unsigned int size);
    Point& Element(unsigned int index);
};

#endif // OUTLINE_H
