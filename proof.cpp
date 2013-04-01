#include "proof.h"
#include "screenpolygon.h"
#include "triangle.h"
#include "parallelogram.h"
#include "outline.h"
#include <allegro.h>
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

    double dir;
    //dir = drawDirection(p2,p1,t.Element(2));
    length = p1.Distance(p2);
    p3 = getPerpendicular(p1,p2,t.Element(2),length,p1,p2);
    //if(dir)
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
//                 p3.Y = p2.Y + length * horizontalY(p2,t.Element(1));
//                 p3.X = p2.X;
//                 break;
//            case NORTH:
//            case SOUTH:
//                 p3.X = p2.X + length * verticalX(p2,t.Element(1));
//                 p3.Y = p2.Y;
//                 break;
//        }
//    }

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

//    dir = drawDirection(p2,p1,t.Element(1));
    length = p1.Distance(p2);
    p3 = getPerpendicular(p2,p1,t.Element(1),length,p1,p2);
//    if(dir)
//    {
//           slant = t.Element(0).Perpendicular(t.Element(2));
//           p3 = p2.Segment(slant, dir * length);
//    }
//    else
//    {
//        switch(Direction(p1,p2))
//        {
//            case WEST:
//            case EAST:
//                 p3.Y = p2.Y + length * horizontalY(p2,t.Element(1));
//                 p3.X = p2.X;
//                 break;
//            case NORTH:
//            case SOUTH:
//                 p3.X = p2.X + length * verticalX(p2,t.Element(1));
//                 p3.Y = p2.Y;
//                 break;
//        }
//    }

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

    p1 = t.Element(2);//thirds point of triangle
    p2 = t.Element(1);//second point of triangle
    
    double dir;
//    dir = drawDirection(p2,p1,t.Element(0));
    length = p1.Distance(p2);
    p3 = getPerpendicular(p2,p1,t.Element(0),p1,p2);
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
//                 p3.Y = p2.Y + length * horizontalY(p2,t.Element(0));
//                 p3.X = p2.X;
//                 break;
//            case NORTH:
//            case SOUTH:
//                 p3.X = p2.X + length * verticalX(p2,t.Element(0));
//                 p3.Y = p2.Y;
//                 break;
//        }
//    }
    

    Shape = new Parallelogram(p1, p2, p3);
    myShapes[1] = new ScreenPolygon(Shape);
    myShapes[1]->Color = getShapeColor(RIGHT_ACUTE,1);//red

    //continuing to write outline
    outline->Element(0) = Shape->Element(2);
    outline->Element(1) = Shape->Element(3);

    /******************
    create first triangle
    *******************/

    p1 = Shape->Element(1);
    p2 = Shape->Element(2);

    length = t.Element(0).Distance(t.Element(1));
    p3 = getPerpendicular(p1,p2,t.Element(0),length,p1,t.Element(0));
//    dir = drawDirection(t.Element(0),t.Element(1),t.Element(2));
//    if(dir)
//    {
//           slant = t.Element(0).Perpendicular(t.Element(1));
//           p3 = p2.Segment(slant, dir * length);
//    }
//    else
//    {
//        switch(Direction(t.Element(0),t.Element(1)))
//        {
//            case WEST:
//            case EAST:
//                 p3.Y = p2.Y + length * horizontalY(p2,Shape->Element(3));
//                 p3.X = p2.X;
//                 break;
//            case NORTH:
//            case SOUTH:
//                 p3.X = p2.X + length * verticalX(p2,Shape->Element(3));
//                 p3.Y = p2.Y;
//                 break;
//        }
//    }

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

    p1 = t.Element(0);
    p2 = t.Element(2);

//    dir = drawDirection(p2,p1,t.Element(1));
    length = t.Element(0).Distance(t.Element(1));
    p3 = getPerpendicular(p2,p1,t.Element(1),distance,p1,t.Element(1));
//    switch(Direction(p1,t.Element(1))) {
//        case NORTHWEST:
//        case NORTHEAST:
//        case SOUTHWEST:
//        case SOUTHEAST:
//            if(dir) { 
//                slant = t.Element(0).Perpendicular(t.Element(1));
//                p3 = p2.Segment(slant, dir * length);
//                break;
//            }
//            else {
//                
//            }
//        case WEST:
//        case EAST:
//            p3.Y = p2.Y - (t.Element(2).Y - t.Element(0).Y);
//            p3.X = p2.X;
//            break;
//        case NORTH:
//        case SOUTH:
//            p3.X = p2.X  - (t.Element(2).X - t.Element(0).X);
//            p3.Y = p2.Y;
//            break;
//   }
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
    p2 = Shape->Element(2);
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

    double dir;
//    dir = drawDirection(p1,p2,t.Element(1));
    length = t.Element(0).Distance(t.Element(2));
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

    dir = drawDirection(t.Element(0),t.Element(1),t.Element(2));
    length = t.Element(0).Distance(t.Element(1));
    if(dir)
    {
           slant = p1.Perpendicular(t.Element(0));
           p3 = p2.Segment(slant, dir * length);
    }
    else
    {
        switch(Direction(p1,t.Element(0)))
        {
            case WEST:
            case EAST:
                 p3.Y = p2.Y + length * horizontalY(p1,p2);
                 p3.X = p2.X;
                 break;
            case NORTH:
            case SOUTH:
                 p3.X = p2.X + length * verticalX(p1,p2);
                 p3.Y = p2.Y;
                 break;
        }
    }

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
    dir = drawDirection(p1,p2,t.Element(0));
    length = p1.Distance(p2);
    if(dir)
    {
           //cout << "dir: " << dir << ", length: " << length << ", slope: " << slant << ", dir: " << Direction(p1,p2) << endl;
           slant = p1.Perpendicular(p2);
           p3 = p2.Segment(slant, dir * length);
    }
    else
    {
        switch(Direction(p1,p2))
        {
            case WEST:
            case EAST:
                 p3.Y = p1.Y + length * horizontalY(p1,t.Element(0));
                 p3.X = p1.X;
                 break;
            case NORTH:
            case SOUTH:
                 p3.X = p1.X + length * verticalX(p1,t.Element(0));
                 p3.Y = p1.Y;
                 break;
        }
    }

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
    
    dir = drawDirection(t.Element(1),t.Element(2),t.Element(0));
    length = t.Element(0).Distance(t.Element(2));
    //cout<<"length: "<<horizontalY(p1,Shape->Element(2))<<endl;
    if(dir)
    {
           //cout<<"dir not zero"<<endl;
       slant = t.Element(0).Perpendicular(t.Element(2));
       p3 = p1.Segment(slant, dir * length);
    }
    else
    {
        switch(Direction(p1,p2))
        {
            case WEST:
            case EAST:
                 p3.Y = p1.Y + length * horizontalY(p1,Shape->Element(2));
                 p3.X = p1.X;
                 break;
            case NORTH:
            case SOUTH:
                 p3.X = p1.X + length * verticalX(p1,Shape->Element(2));
                 p3.Y = p1.Y;
                 break;
        }

    }
    //cout<<"end"<<endl;
//    if(t.Element(0).X - t.Element(2).X == 0)//infinite slant
//    {
//        p2.Y = p1.Y;
//        p2.X = p1.X + length;
//    }
//    else if(t.Element(0).Y - t.Element(2).Y == 0)//0slant
//    {
//        p2.X = p1.X;
//        p2.Y = p1.Y - length;
//    }
//    else
//    {
//        slant = t.Element(0).Perpendicular(t.Element(2));
//        p2 = p1.Segment(slant, -length);
//    }

//    p3 = t.Element(2);

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
//double dir = drawDirection(p1,p2,element(0));
//if(dir)
//       Segment(slope, dir * distance)
//else
//{
//    
//}
//double Proof::drawDirection(const Point& p1, const Point& p2, const Point& p3) {
//       DIR dir1 = Direction(p1,p2);
//       DIR dir2 = Direction(p1,p3);
//       switch(dir1) {
//       case NORTH:
//       case SOUTH:
//       case EAST:
//       case WEST:
//            return 0;
//       case NORTHWEST:
//            switch(dir2) {
//                case NORTHWEST:
//                     return abs(p1.Slope(p2) > p1.Slope(p3)) ? 1: -1;
//                case WEST:
//                case SOUTHWEST:
//                case SOUTH:
//                     return 1;
//                case NORTH:
//                case NORTHEAST:
//                case EAST:
//                case SOUTHEAST:
//                     return -1;
//            }
//            break;
//       case SOUTHWEST:
//            switch(dir2) {
//            case SOUTHWEST:
//                 return abs(p1.Slope(p2) < p1.Slope(p3)) ? 1: -1;
//             case WEST:
//             case NORTHWEST:
//             case NORTH:
//                  return 1;
//             case SOUTH:
//             case SOUTHEAST:
//             case EAST:
//             case NORTHEAST:
//                  return -1;
//            }
//            break;
//       case SOUTHEAST:
//            switch(dir2) {
//                 case SOUTHEAST:
//                      return abs(p1.Slope(p2) > p1.Slope(p3)) ? -1 : 1;
//                 case EAST:
//                 case NORTHEAST:
//                 case NORTH:
//                      return -1;
//                 case NORTHWEST:
//                 case WEST:
//                 case SOUTHWEST:
//                 case SOUTH:
//                      return 1;
//            }
//       case NORTHEAST:
//            switch(dir2) {
//                 case NORTHEAST:
//                      return abs(p1.Slope(p2) < p1.Slope(p3)) ? -1 : 1;
//                 case EAST:
//                 case SOUTHEAST:
//                 case SOUTH:
//                      return -1;
//                 case NORTHWEST:
//                 case WEST:
//                 case SOUTHWEST:
//                 case NORTH:
//                      return 1;
//            }     
//       }
//}

//double Proof::verticalX(const Point& p1, const Point& p3) {
//    switch(Direction(p1,p3)) {
//    case WEST:
//    case SOUTHWEST:
//    case NORTHWEST:
//        return 1;
//    case EAST:
//    case SOUTHEAST:
//    case NORTHEAST:
//        return -1;
//    }
//}
//
//double Proof::horizontalY(const Point& p1, const Point& p3) {
//    switch(Direction(p1,p3)) {
//    case NORTHEAST:
//    case NORTH:
//    case NORTHWEST:
//        return 1;
//    case SOUTHEAST:
//    case SOUTH:
//    case SOUTHWEST:
//        return -1;
//    }
//}


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
Point getPerpendicular(Point vertex, Point shared, Point unshared, double distance, Point slopeStart, Point slopeStop) {
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
      switch(common) {
       case NORTH:
            switch(uncommon) {
            case NORTHEAST:
            case EAST:
            case SOUTHEAST:
                 distance *= -1;
            }
       case SOUTH:
            switch(uncommon) {
            case SOUTHEAST:
            case EAST:
            case NORTHEAST:
                 distance *= -1;
            }
       case WEST:
       case EAST:
            switch(uncommon) {
            case NORTH:
            case NORTHEAST:
            case NORTHWEST:
                 distance *= -1;
            }
       case NORTHEAST:
            switch(uncommon) {
            case EAST:
            case SOUTHEAST:
            case SOUTH:
                 distance *= -1;
                 break;
            case NORTHEAST:
                 if(abs(drawSlope) < abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            case SOUTHWEST:
                 if(abs(drawSlope) > abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            }
       case NORTHWEST:
            switch(uncommon) {
            case EAST:
            case NORTHEAST:
            case NORTH:
                 distance *= -1;
                 break;
            case NORTHWEST:
                 if(abs(drawSlope) > abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            case SOUTHEAST:
                 if(abs(drawSlope) < abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            }
       case SOUTHEAST:
            switch(uncommon) {
            case EAST:
            case NORTHEAST:
            case NORTH:
                 distance *= -1;
                 break;
            case SOUTHEAST:
                 if(abs(drawSlope) < abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            case NORTHWEST:
                 if(abs(drawSlope) > abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            }
       case SOUTHWEST:
            switch(uncommon) {
            case EAST:
            case SOUTHEAST:
            case SOUTH:
                 distance *= -1;
                 break;
            case SOUTHWEST:
                 if(abs(drawSlope) > abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            case NORTHWEST:
                 if(abs(drawSlope) < abs(vertex.Slope(common)))
                      distance *= -1;
                 break;
            }
      }
      Point result;
      if(drawSlope)
           result = vertex.Segment(drawSlope,distance);
      else {
           if(horizontal) {
                result.Y = vertex.Y;
                result.X = vertex.X + distance;
           }
           else {
                result.X = vertex.X;
                result.Y = vertex.Y + distance;
           }
      }
}
