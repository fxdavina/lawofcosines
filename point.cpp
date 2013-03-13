//
//  Point.cpp
//  Polygon
//
//  Created by Arpineh Asadoorian on 2/18/13.
//  Copyright (c) 2013 Arpineh Asadoorian. All rights reserved.
//

#include "Point.h"
Point::Point()
{
    x=0;
    y=0;
}
Point::Point(int X, int Y)
{
    x=X;
    y=Y;
}
Point::~Point()
{
    
}
int Point::getX() const
{
    return x;
}

int Point::getY() const
{
    return y;
}

void Point::Set(int X, int Y)
{
    
    x=X;
    y=Y;
}
void Point::SetX(int X)
{
    x=X;

}
void Point::SetY(int Y)
{
    y=Y;
}
Point& Point::operator=(const Point& other)
{
    if(this != &other)
        copy(other);
    return *this;
}
void Point::copy (const Point&  other)
{
    x = other.x;
    y = other.y;
}
