#ifndef RIGHTACUTE_H
#define RIGHTACUTE_H

#include "proof.h"
#include "triangle.h"

class RightAcute: Proof {
public:
    RightAcute(const Triangle& t, unsigned int angle);
};

#endif // RIGHTACUTE_H
