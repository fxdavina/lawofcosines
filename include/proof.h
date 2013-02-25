#ifndef PROOF_H
#define PROOF_H
#include "screenpolygon.h"

class Proof {
public:
    Proof(unsigned int numPolygons, unsigned int numVertices);
    Proof(const Proof& other);
    ~Proof();
    Proof& operator=(const Proof& other);
    ScreenPolygon Shape(unsigned int index);
    ScreenPolygon Outline() const;
    double MinX() const;
    double MaxX() const;
    double MinY() const;
    double MaxY() const;
    double Height() const;
    double Width() const;
protected:
    ScreenPolygon& shape(unsigned int index);
    ScreenPolygon& outline();
};

#endif // PROOF_H
