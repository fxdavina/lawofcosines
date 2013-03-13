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

class Proof {
public:
    Proof(PROOF_TYPE type, const Triangle& tri);
    Proof(const Proof& other);
    ~Proof();
    Proof& operator=(const Proof& other);
    ScreenPolygon& Shape(unsigned int index) const;
    ScreenPolygon& Outline() const;
    double MinX() const;
    double MinY() const;
    double Height() const;
    double Width() const;
    unsigned int Count() const;
    double Scale;
    void Translate(double X, double Y);
private:
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
