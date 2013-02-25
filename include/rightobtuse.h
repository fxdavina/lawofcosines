#ifndef RIGHTOBTUSE_H
#define RIGHTOBTUSE_H

#include "proof.h"
#include "triangle.h"

class RightObtuse: Proof {
public:
    RightObtuse(const Triangle& t, unsigned int angle);
};

#endif // RIGHTOBTUSE_H
