#include "outline.h"

Outline::Outline(unsigned int size):Polygon(size) {}
Point& Outline::Element(unsigned int index) {
    return Polygon::element(index);
}