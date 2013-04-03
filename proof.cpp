#include "proof.h"
#include "screenpolygon.h"
#include "triangle.h"
#include "parallelogram.h"
#include "outline.h"
#include <allegro.h>

#define DEBUG_PROOF
#ifdef DEBUG_PROOF
#include <fstream>
#include <string>
#include <sstream>
void debugProof(std::string message) {
     std::ofstream out;
     out.open("debug.txt",std::ios::app);
     out << message << std::endl;
     out.close();
}

#endif

using namespace std;
int getShapeColor(PROOF_TYPE t,unsigned int index);

Proof::~Proof()
{
    delete[]myShapes;
    delete myOutline;
}

Proof::Proof(const Proof& other)
{
    minX = other.minX;
    minY = other.minY;
    width = other.width;
    height = other.height;
    numPolygons = other.numPolygons;

    Outline* outline = new Outline(other.myOutline->Shape().Size());
    myOutline = new ScreenPolygon((Polygon*)outline);
    myShapes = new ScreenPolygon*[numPolygons];

    for(int i=0; i<numPolygons; i++)
    {
        myShapes[i] = other.myShapes[i];
    }                   
}

Proof &Proof::operator =(const Proof &other)
{
    minX = other.minX;
    minY = other.minY;
    width = other.width;
    height = other.height;
    numPolygons = other.numPolygons;

    Outline* outline = new Outline(other.myOutline->Shape().Size());
    myOutline = new ScreenPolygon((Polygon*)outline);
    myShapes = new ScreenPolygon*[numPolygons];

    for(int i=0; i<numPolygons; i++)
    {
        myShapes[i] = other.myShapes[i];
    }

    return *this;
}
Proof::Proof(PROOF_TYPE type, const Triangle& tri)
{
                        #ifdef DEBUG_PROOF
                        debugProof("Create Proof");
                        #endif
    switch(type)
    {
        case LEFT_ACUTE:
            doLeftAcute(tri);
            break;
        case RIGHT_ACUTE:
            doRightAcute(tri);
            break;
        case LEFT_OBTUSE:
            doLeftObtuse(tri);
            break;
        case RIGHT_OBTUSE:
            doRightObtuse(tri);
            break;
    }
}

double Proof::MinX() const
{    
    return minX;
}
double Proof::MinY() const
{
    return minY;
}
double Proof::Height() const
{
    return height;
}
double Proof::Width() const
{
    return width;
}

unsigned int Proof::Count() const
{
    return numPolygons;
}

ScreenPolygon &Proof::Shape(unsigned int index) const
{
    return *myShapes[index];
}

ScreenPolygon &Proof::GetOutline() const
{
    return *myOutline;
}

void Proof::Scale(double S)
{
    for(int i=0; i<numPolygons; i++)
    {
        for(int j=0; j< myShapes[i]->Shape().Size(); j++)
        {
            myShapes[i]->Shape().Element(j).X *= S;
            myShapes[i]->Shape().Element(j).Y *= S;
        }
    }
}

void Proof::Translate(double X, double Y)
{
    for(int i=0; i<numPolygons; i++)
    {
        myShapes[i]->Shape().Translate(X,Y);
    }
}

void Proof::doLeftAcute(const Triangle& t)
{
    numPolygons = 5;//5 total polygons
    myShapes = new ScreenPolygon*[5];//5 polygons in leftAcute

    Outline* outline = new Outline(7);//set an outline with 7 points
    myOutline = new ScreenPolygon((Polygon*)outline);

    //put the base triangle into myShapes
    Polygon* Shape;
    Shape = new Triangle(t.Element(0), t.Element(1), t.Element(2));
    myShapes[0] = new ScreenPolygon(Shape);
    myShapes[0]->Color = getShapeColor(LEFT_ACUTE,0); //blue

    //starting to set up outline
    outline->Element(0) = t.Element(0);
    outline->Element(1) = t.Element(1);

    //items needed to create polygons
    Point p1, p2, p3; // 3 points are needed to make a parallelogram/triangle
    double length;//length for any polygon
    double slant;//slant to determine segment
    
    /******************
    create first square
    *******************/

    p1 = t.Element(1);
    p2 = t.Element(0);

    length = p1.Distance(p2);
    p3 = getPerpendicular(p2,p1,t.Element(2),length,p1,p2);

    Shape = new Parallelogram(p1, p2, p3);
    myShapes[1] = new ScreenPolygon(Shape);
    myShapes[1]->Color = getShapeColor(LEFT_ACUTE,1);//red

    //continuing to write outline.
    outline->Element(5) = Shape->Element(2);
    outline->Element(6) = Shape->Element(3);

    /******************
    create second square
    *******************/

    p1 = t.Element(0);//thirds point of triangle
    p2 = t.Element(2);//second point of triangle

    length = p1.Distance(p2);
    p3 = getPerpendicular(p2,p1,t.Element(1),length,p1,p2);

    Shape = new Parallelogram(p1, p2, p3);
    myShapes[2] = new ScreenPolygon(Shape);
    myShapes[2]->Color = getShapeColor(LEFT_ACUTE,2);//green

    //continuing to write outline.
    outline->Element(2) = Shape->Element(2);
    outline->Element(3) = Shape->Element(3);

    /******************
    create first triangle
    *******************/
    Parallelogram temp(myShapes[1]->Shape().Element(2),t.Element(0),myShapes[2]->Shape().Element(3));
    
    p1 = t.Element(0);
    p2 = Shape->Element(3);
    p3 = temp.Element(3);

    Shape = new Triangle(p1,p2,p3);
    myShapes[3] = new ScreenPolygon(Shape);
    myShapes[3]->Color = getShapeColor(LEFT_ACUTE,3);//pink

    //finish the last point of the outline.
    outline->Element(4) = Shape->Element(2);

    /******************
    create Second triangle
    *******************/

    p2 = myShapes[1]->Shape().Element(2);

    Shape = new Triangle(p1,p2,p3);
    myShapes[4] = new ScreenPolygon(Shape);
    myShapes[4]->Color = getShapeColor(LEFT_ACUTE,4);//orange
    
    width = minX = outline->Element(0).X;
    height = minY = outline->Element(0).Y;
    
    for(int i=0; i<numPolygons; i++)
    {
        for(int j=0; j< myShapes[i]->Shape().Size(); j++)
        {
            if(myShapes[i]->Shape().Element(j).X < minX)
                minX=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y < minY)
                minY =myShapes[i]->Shape().Element(j).Y; 
            if(myShapes[i]->Shape().Element(j).X > width)
                width=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y > height)
                height =myShapes[i]->Shape().Element(j).Y;
        }
    }
    width = width-minX;
    height = height-minY;

}

void Proof::doRightAcute(const Triangle& t)
{
    numPolygons = 6;//6 total polygons
    myShapes = new ScreenPolygon*[6];//6 polygons in RightAcute

    Outline* outline = new Outline(7);//set an outline with 7 points
    myOutline = new ScreenPolygon((Polygon*)outline);

    Polygon* Shape;
    Shape = new Triangle(t.Element(0), t.Element(1), t.Element(2));
    myShapes[0] = new ScreenPolygon(Shape);//set Main Triangle into myShapes
    myShapes[0]->Color = getShapeColor(RIGHT_ACUTE,0); //blue

    //start outline
    outline->Element(4) = t.Element(0);

    Point p1, p2, p3; // 3 points are needed to make a parallelogram/triangle
    double length;//length for any polygon
    double slant;//slant to determine segment

    /******************
    create first square
    *******************/
                        #ifdef DEBUG_PROOF
                        debugProof("\tFirst Square");
                        #endif    
    p1 = t.Element(2);//thirds point of triangle
    p2 = t.Element(1);//second point of triangle
    
    length = p1.Distance(p2);
    p3 = getPerpendicular(p2,p1,t.Element(0),length,p1,p2);


    Shape = new Parallelogram(p1, p2, p3);
    myShapes[1] = new ScreenPolygon(Shape);
    myShapes[1]->Color = getShapeColor(RIGHT_ACUTE,1);//red

    //continuing to write outline
    outline->Element(0) = Shape->Element(2);
    outline->Element(1) = Shape->Element(3);

    /******************
    create first triangle
    *******************/
                        #ifdef DEBUG_PROOF
                        debugProof("\tFirst Triangle");
                        #endif    

    p1 = Shape->Element(1);
    p2 = Shape->Element(2);
    Point horiz(p2.X + t.Element(1).X - t.Element(0).X,
                p2.Y + t.Element(1).Y - t.Element(0).Y);
    length = t.Element(0).Distance(t.Element(1));
    p3 = getPerpendicular(p2,horiz,Shape->Element(3),length,t.Element(1),t.Element(0));

    Shape = new Triangle(p1,p2,p3);
    myShapes[2] = new ScreenPolygon(Shape);
    myShapes[2]->Color = getShapeColor(RIGHT_ACUTE,2);//pink

    //continuing to write outline
    outline->Element(6) = Shape->Element(2);

    /******************
    create first parallelogram
    *******************/

    p1 = Shape->Element(2);
    p2 = t.Element(1);
    p3 = t.Element(0);

    Shape = new Parallelogram(p1,p2,p3);
    myShapes[3] = new ScreenPolygon(Shape);
    myShapes[3]->Color = getShapeColor(RIGHT_ACUTE,3);//yellow

    //continuing to write outline
    outline->Element(5) = Shape->Element(3);

    /******************
    create second parallelogram
    *******************/
                        #ifdef DEBUG_PROOF
                        debugProof("\tSecond Parallelogram");
                        #endif    

    p1 = t.Element(2);
    p2 = t.Element(0);
    horiz.X = p2.X + t.Element(0).X - t.Element(1).X;
    horiz.Y = p2.Y + t.Element(0).Y - t.Element(1).Y;
    length = t.Element(0).Distance(t.Element(1));
    p3 = getPerpendicular(p2,horiz,Shape->Element(3),length,t.Element(0),t.Element(1));

    Shape = new Parallelogram(p1,p2,p3);
    myShapes[4] = new ScreenPolygon(Shape);
    myShapes[4]->Color = getShapeColor(RIGHT_ACUTE,4);//purple

    //done writing outline
    outline->Element(2) = Shape->Element(2);
    outline->Element(3) = Shape->Element(3);

    /******************
    create second triangle
    *******************/

    p1 = t.Element(2);
    p2 = Shape->Element(3);
    p3 = myShapes[1]->Shape().Element(3);

    Shape = new Triangle(p1,p2,p3);
    myShapes[5] = new ScreenPolygon(Shape);
    myShapes[5]->Color = getShapeColor(RIGHT_ACUTE,5);//orange
    
    width = minX = outline->Element(0).X;
    height = minY = outline->Element(0).Y;
    
    for(int i=0; i<numPolygons; i++)
    {
        for(int j=0; j< myShapes[i]->Shape().Size(); j++)
        {
            if(myShapes[i]->Shape().Element(j).X < minX)
                minX=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y < minY)
                minY =myShapes[i]->Shape().Element(j).Y; 
            if(myShapes[i]->Shape().Element(j).X > width)
                width=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y > height)
                height =myShapes[i]->Shape().Element(j).Y;
        }
    }
    width = width-minX;
    height = height-minY;
}

void Proof::doLeftObtuse(const Triangle& t)
{
    numPolygons = 6;//6 total polygons
    myShapes = new ScreenPolygon*[6];//6 polygons in RightAcute

    Point p1, p2, p3; // 3 points are needed to make a parallelogram/triangle
    double length;//length for any polygon
    double slant;//slant to determine segment

    Outline* outline = new Outline(6);//set an outline with 6 points
    myOutline = new ScreenPolygon((Polygon*)outline);

    Polygon* Shape;
    Shape = new Triangle(t.Element(0), t.Element(1), t.Element(2));
    myShapes[0] = new ScreenPolygon(Shape);//set Main Triangle into myShapes
    myShapes[0]->Color = getShapeColor(LEFT_OBTUSE,0); //blue

    //start outline
    outline->Element(0) = t.Element(1);
    outline->Element(1) = t.Element(2);

    /******************
    create first square
    *******************/

    p1 = t.Element(2);
    p2 = t.Element(0);

    length = t.Element(0).Distance(t.Element(2));
    p3 = getPerpendicular(p2,p1,t.Element(1),length,p1,p2);
//    if(dir)
//    {
//           slant = p1.Perpendicular(p2);
//           p3 = p2.Segment(slant, dir * length);
//    }
//    else
//    {
//        switch(Direction(p1,p2))
//        {
//            case WEST:
//            case EAST:
//                 p3.Y = p2.Y + length * horizontalY(p1,t.Element(1));
//                 p3.X = p2.X;
//                 break;
//            case NORTH:
//            case SOUTH:
//                 p3.X = p2.X + length * verticalX(p1,t.Element(1));
//                 p3.Y = p2.Y;
//                 break;
//        }
//    }

    Shape = new Parallelogram(p1,p2,p3);
    myShapes[1] = new ScreenPolygon(Shape);
    myShapes[1]->Color = getShapeColor(LEFT_OBTUSE,1); //cardinal red

    //continuing outline
    outline->Element(2) = Shape->Element(3);

    /******************
    create first parallelogram
    *******************/

    p1 = t.Element(1);
    p2 = t.Element(0);
    p3 = Shape->Element(2);

    Shape = new Parallelogram(p1,p2,p3);
    myShapes[2] = new ScreenPolygon(Shape);
    myShapes[2]->Color = getShapeColor(LEFT_OBTUSE,2);//yellow

    /******************
    create first triangle
    *******************/
    p1 = t.Element(1);
    p2 = Shape->Element(3);

    length = t.Element(0).Distance(t.Element(1));
    p3 = getPerpendicular(Shape->Element(3),Shape->Element(2),Shape->Element(0),length,t.Element(1),t.Element(0));

    Shape = new Triangle(p1,p2,p3);
    myShapes[3] = new ScreenPolygon(Shape);
    myShapes[3]->Color = getShapeColor(LEFT_OBTUSE,3);//purple

    //continuing outline
    outline->Element(5) = Shape->Element(2);

    /******************
    create second square
    *******************/

    p1 = Shape->Element(2);
    p2 = Shape->Element(1);
    p3 = myShapes[1]->Shape().Element(2);

    Shape = new Parallelogram(p1,p2,p3);
    myShapes[4] = new ScreenPolygon(Shape);
    myShapes[4]->Color = getShapeColor(LEFT_OBTUSE,4);//green

    //continuing outline
    outline->Element(4)=Shape->Element(3);

    /******************
    create second parallelogram
    *******************/

    p1 = Shape->Element(3);
    p2 = Shape->Element(2);
    p3 = myShapes[1]->Shape().Element(3);
    
    Shape = new Parallelogram(p1,p2,p3);
    myShapes[5] = new ScreenPolygon(Shape);
    myShapes[5]->Color = getShapeColor(LEFT_OBTUSE,5);//purple

    //finishing outline
    outline->Element(3) = Shape->Element(3);
    
    width = minX = outline->Element(0).X;
    height = minY = outline->Element(0).Y;
    
    for(int i=0; i<numPolygons; i++)
    {
        for(int j=0; j< myShapes[i]->Shape().Size(); j++)
        {
            if(myShapes[i]->Shape().Element(j).X < minX)
                minX=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y < minY)
                minY =myShapes[i]->Shape().Element(j).Y; 
            if(myShapes[i]->Shape().Element(j).X > width)
                width=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y > height)
                height =myShapes[i]->Shape().Element(j).Y;
        }
    }
    width = width-minX;
    height = height-minY;
}

void Proof::doRightObtuse(const Triangle& t)
{
    Point p1, p2, p3; // 3 points are needed to make a parallelogram/triangle
    double length;//length for any polygon
    double slant;//slant to determine segment

    numPolygons = 3; //3 total polygons
    myShapes = new ScreenPolygon*[3];//3 polygons in rightObtuse

    Outline* outline = new Outline(6);//set an outline with 6 points
    myOutline = new ScreenPolygon((Polygon*)outline);

    Polygon* Shape;
    Shape = new Triangle(t.Element(0), t.Element(1), t.Element(2));
    myShapes[0] = new ScreenPolygon(Shape);//set Main Triangle into myShapes
    myShapes[0]->Color = getShapeColor(RIGHT_OBTUSE,0); //blue

    //writing outline
    outline->Element(0) = t.Element(1);
    outline->Element(4) = t.Element(2);
    outline->Element(5) = t.Element(0);

    /******************
    create first square
    *******************/
  
    p1 = t.Element(2);
    p2 = t.Element(1);

    
    double dir;

    length = p1.Distance(p2);
    p3 = getPerpendicular(p2,p1,t.Element(0),length,p1,p2);

    Shape = new Parallelogram(p1,p2,p3);
    myShapes[1] = new ScreenPolygon(Shape);
    myShapes[1]->Color = getShapeColor(RIGHT_OBTUSE,1);//red

    //continue with outline
    outline->Element(1) = Shape->Element(2);
    outline->Element(2) = Shape->Element(3);

    /******************
    create first triangle
    *******************/

    p1 = Shape->Element(3);
    p2 = Shape->Element(0);
    
    length = t.Element(0).Distance(t.Element(2));
    p3 = getPerpendicular(p1,p2,Shape->Element(2),length,t.Element(0),t.Element(2));


    Shape = new Triangle(p1,p2,p3);
    myShapes[2] = new ScreenPolygon(Shape);
    myShapes[2]->Color = getShapeColor(RIGHT_OBTUSE,2);//pink
    //cout<<"after myShapes"<<endl;
    //done with outline
    outline->Element(3) = Shape->Element(1);
    //cout<<"after outline"<<endl;
    width = minX = outline->Element(0).X;
    height = minY = outline->Element(0).Y;
    //cout<<"after width and height"<<endl;
    for(int i=0; i<numPolygons; i++)
    {
        for(int j=0; j< myShapes[i]->Shape().Size(); j++)
        {
            if(myShapes[i]->Shape().Element(j).X < minX)
                minX=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y < minY)
                minY =myShapes[i]->Shape().Element(j).Y; 
            if(myShapes[i]->Shape().Element(j).X > width)
                width=myShapes[i]->Shape().Element(j).X;
            if(myShapes[i]->Shape().Element(j).Y > height)
                height =myShapes[i]->Shape().Element(j).Y;
        }
    }
    width = width-minX;
    height = height-minY;
    //cout<<"after all"<<endl;
}

DIR Proof::Direction(const Point& p1, const Point& p2) {
    double xDiff = p2.X - p1.X;
    double yDiff = p2.Y - p1.Y;
    if(yDiff == 0) {
        return (xDiff > 0) ? EAST : WEST;
    }
    if(xDiff == 0) {
             return (yDiff > 0) ? SOUTH : NORTH;
    }
    double m = p1.Slope(p2);
    if(m < 0) {
         return (xDiff > 0) ? NORTHEAST : SOUTHWEST;
    }
    else {
         return (xDiff > 0) ? SOUTHEAST : NORTHWEST;
    }
    
}

int getShapeColor(PROOF_TYPE t,unsigned int index) {
     int result = makecol(64,64,255);
     if(index) {
          switch(t) {
          case LEFT_ACUTE:
          case LEFT_OBTUSE:
               result = makecol(255,(40 * index),(40 * index));
               break;
          case RIGHT_ACUTE:
          case RIGHT_OBTUSE:
               result = makecol(64,255 - (30 * index),64);
               break;
          }
     }
     return result;
}
Point Proof::getPerpendicular(Point vertex, Point shared, Point unshared, double distance, Point slopeStart, Point slopeStop) {
      DIR common, uncommon, draw;
      bool horizontal;
      double drawSlope = 0;
      common = Direction(vertex,shared);
      uncommon = Direction(vertex,unshared);
      draw = Direction(slopeStart,slopeStop);
      switch(draw) {
      case NORTH:
      case SOUTH:
           horizontal = true;
           break;
      case EAST:
      case WEST:
           horizontal = false;
           break;
      default:
           drawSlope = slopeStart.Perpendicular(slopeStop);
      }
      #ifdef DEBUG_PROOF
      std::stringstream ss;
      ss << "\t\tCOMMON: " << common;
      if(vertex.X != shared.X)
                  ss << "(" << vertex.Slope(shared) << ") ";
      ss << ", UNCOMMON: " << uncommon;
            if(vertex.X != unshared.X)
                  ss << "(" << vertex.Slope(unshared) << ") ";
      ss << ",DRAW: " << draw << ", SLOPE: " << drawSlope;
      debugProof(ss.str());
      #endif 
      switch(common) {
       case NORTH:
       case SOUTH:
            switch(uncommon) {
            case SOUTHEAST:
            case EAST:
            case NORTHEAST:
                 distance *= -1;
            }
            break;
       case WEST:
       case EAST:
            switch(uncommon) {
            case NORTH:
            case NORTHEAST:
            case NORTHWEST:
                 distance *= -1;
            }
            break;
       case NORTHEAST:
            switch(uncommon) {
            case EAST:
            case SOUTHEAST:
            case SOUTH:
                 distance *= -1;
                 break;
            case NORTHEAST:
                 if(fabs(vertex.Slope(unshared)) < fabs(vertex.Slope(shared)))
                      distance *= -1;
                 break;
            case SOUTHWEST:
                 if(fabs(vertex.Slope(unshared)) > fabs(vertex.Slope(shared)))
                      distance *= -1;
                 break;
            }
            break;
       case NORTHWEST:
            switch(uncommon) {
            case EAST:
            case NORTHEAST:
            case NORTH:
                 distance *= -1;
                 break;
            case NORTHWEST:
                 if(fabs(vertex.Slope(unshared)) > fabs(vertex.Slope(shared)))
                      distance *= -1;
                 break;
            case SOUTHEAST:
                 if(fabs(vertex.Slope(unshared)) < fabs(vertex.Slope(shared)))
                      distance *= -1;
                 break;
            }
            break;
       case SOUTHEAST:
            switch(uncommon) {
            case EAST:
            case NORTHEAST:
            case NORTH:
                 distance *= -1;
                 break;
            case SOUTHEAST:
                #ifdef DEBUG_PROOF
                ss.str("");
                ss << "\t\tDOUBLE SE: " << distance;
                #endif 
                if(fabs(vertex.Slope(unshared)) < fabs(vertex.Slope(shared))) {
                      distance *= -1;
                      #ifdef DEBUG_PROOF
                        ss << "->" << distance;
                        #endif
                }
                #ifdef DEBUG_PROOF
                debugProof(ss.str());
                #endif
                 break;
            case NORTHWEST:
                 if(fabs(vertex.Slope(unshared)) > fabs(vertex.Slope(shared)))
                      distance *= -1;
                 break;
            }
            break;
       case SOUTHWEST:
            switch(uncommon) {
            case EAST:
            case SOUTHEAST:
            case SOUTH:
                 distance *= -1;
                 break;
            case SOUTHWEST:
                 if(fabs(vertex.Slope(unshared)) > fabs(vertex.Slope(shared)))
                      distance = -distance;
                 break;
            case NORTHEAST:
                 if(fabs(vertex.Slope(unshared)) < fabs(vertex.Slope(shared)))
                      distance *= -1;
                 break;
            }
            break;
      }
      #ifdef DEBUG_PROOF
      ss.str("");
      ss << "\t\tDISTANCE: " << distance;
      debugProof(ss.str());
      #endif 
      Point result;
      if(drawSlope) {
                    #ifdef DEBUG_PROOF
           debugProof("\t\tCalc Valid Slope");
           #endif
           result = vertex.Segment(drawSlope,distance);
      }
      else {
                                   #ifdef DEBUG_PROOF
                        debugProof("\t\tCalc other slope");
                        #endif
           if(horizontal) {
                result.Y = vertex.Y;
                result.X = vertex.X + distance;
           }
           else {
                result.X = vertex.X;
                result.Y = vertex.Y - distance;
           }
      }
      return result;
}
