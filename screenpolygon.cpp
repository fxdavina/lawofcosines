#include <iostream>
#include <cstdlib>
#include <allegro.h>
#include "ScreenPolygon.h"
#include "Polygon.h"

using namespace std;

ScreenPolygon::ScreenPolygon(Polygon* p)
{
  poly = p;
  Scale = 1;
  Color = RGB(255,255,255);
}

ScreenPolygon::ScreenPolygon(const ScreenPolygon &other)
{
  poly = new Polygon(*other.poly);
  Scale = other.Scale;
  Color = other.Color;
}

ScreenPolygon::~ScreenPolygon()
{
  delete poly;
}

ScreenPolygon& ScreenPolygon::operator=(const ScreenPolygon& other)
{
  if(this != &other){
      delete poly;
      poly = new Polygon(*other.poly);
      Scale = other.Scale;
      Color = other.Color;
  }
  return *this;
}

void ScreenPolygon::Draw(BITMAP* buffer) const
{
  if(poly->Size() == 3)
  {
    triangle(buffer, poly->Element(0).X, poly->Element(0).Y, poly->Element(1).X,
             poly->Element(1).Y, poly->Element(2).X,poly->Element(2).Y, Color);
  }
  if(poly->Size() == 4)
  {
    triangle(buffer, poly->Element(0).X, poly->Element(0).Y, poly->Element(1).X,
             poly->Element(1).Y, poly->Element(2).X,poly->Element(2).Y, Color)
    triangle(buffer, poly->Element(0).X, poly->Element(0).Y, poly->Element(3).X,
             poly->Element(3).Y, poly->Element(2).X,poly->Element(2).Y, Color)
  }
  if(poly->Size() > 4)
  {
    for(int i=0; i<poly->Size()-1; i++)
    {
        line(buffer, poly->Element(i).X, poly->Element(i).Y,
             poly->Element(i + 1).X, poly->Element(i + 1).Y, Color);
    }
    line(buffer, poly->Element(poly->Size()-1).X, poly->Element(poly->Size()-1).Y,
             poly->Element(0).X, poly->Element(0).Y, Color);
  }
}

Polygon& ScreenPolygon::Shape() const
{
  return *poly;
}
