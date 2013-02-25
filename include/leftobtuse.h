#ifndef LEFTOBTUSE_H
#define LEFTOBTUSE_H

#include "proof.h"
#include "triangle.h"

class LeftObtuse: Proof {
public:
    LeftObtuse(const Triangle& t, unsigned int angle);
};

#endif // LEFTOBTUSE_H
