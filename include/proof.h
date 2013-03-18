#ifndef PROOF_H
#define PROOF_H

#include "screenpolygon.h"
#include "triangle.h"
#include "parallelogram.h"

enum PROOF_TYPE {
    LEFT_ACUTE,
    RIGHT_ACUTE,
    LEFT_OBTUSE,
    RIGHT_OBTUSE
};
    enum DIR { NORTH, NORTHWEST, WEST, SOUTHWEST, SOUTH, SOUTHEAST, EAST, NORTHEAST };

class Proof {
public:
    Proof(PROOF_TYPE type, const Triangle& tri);
    Proof(const Proof& other);
    ~Proof();
    Proof& operator=(const Proof& other);
    ScreenPolygon& Shape(unsigned int index) const;
    ScreenPolygon& GetOutline() const;
    double MinX() const;
    double MinY() const;
    double Height() const;
    double Width() const;
    unsigned int Count() const;
    void Scale(double s);
    void Translate(double X, double Y);
private:
    double verticalX(const Point& p1, const Point& p3);
    double horizontalY(const Point& p1, const Point& p3);
    DIR Direction(const Point& p1, const Point& p2);
    double drawDirection(const Point& p1, const Point& p2, const Point& p3);
    void doLeftAcute(const Triangle& t);
    void doRightAcute(const Triangle& t);
    void doLeftObtuse(const Triangle& t);
    void doRightObtuse(const Triangle& t);
    unsigned int numPolygons;
    ScreenPolygon** myShapes;
    ScreenPolygon* myOutline;
    double minX;
    double minY;
    double height;
    double width;
};

#endif // PROOF_H
