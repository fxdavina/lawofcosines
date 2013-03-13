//
//  Point.cpp
//  Polygon
//
//  Created by Arpineh Asadoorian on 2/18/13.
//  Copyright (c) 2013 Arpineh Asadoorian. All rights reserved.
//
#include "outline.h"
Outline::Outline(unsigned int size):Polygon(size) {}
Point& Outline::Element(unsigned int index) {
    return Polygon::element(index);
}