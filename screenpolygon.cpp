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
  Color = makecol(255,255,255);
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
    triangle(buffer,int(poly->Element(0).X), int(poly->Element(0).Y), int(poly->Element(1).X),
             int(poly->Element(1).Y), int(poly->Element(2).X), int(poly->Element(2).Y), Color);
  }
  if(poly->Size() == 4)
  {
    triangle(buffer, int(poly->Element(0).X), int(poly->Element(0).Y), int(poly->Element(1).X),
             int(poly->Element(1).Y), int(poly->Element(2).X), int(poly->Element(2).Y), Color);
    triangle(buffer, int(poly->Element(0).X), int(poly->Element(0).Y), int(poly->Element(3).X),
             int(poly->Element(3).Y), int(poly->Element(2).X), int(poly->Element(2).Y), Color);
  }
  if(poly->Size() > 4)
  {
    for(int i=0; i<poly->Size()-1; i++)
    {
        line(buffer, int(poly->Element(i).X), int(poly->Element(i).Y),
             int(poly->Element(i + 1).X), int(poly->Element(i + 1).Y), Color);
    }
    line(buffer, int(poly->Element(poly->Size()-1).X), int(poly->Element(poly->Size()-1).Y),
             int(poly->Element(0).X), int(poly->Element(0).Y), Color);
  }
}

Polygon& ScreenPolygon::Shape() const
{
  return *poly;
}
