#ifndef SPRING_H
#define SPRING_H

#include "mass.h"
#include "common.hpp"

class Mass;

// A spring modelled after Hooke's law and includes damping
// A spring is connected to Mass objects on each of it's ends
class Spring
{
public:
    // spring constant in newtons/meter
    // rest length in meters
    float k;         // spring constant
    float kd;        // damping constants
    float rl;
    Mass *m1;   // masses on spring's ends
    Mass *m2;
    float dscale;

    Spring(float springConstant, float restLength, Mass *mass1, Mass *mass2);
    vec3 getForce(Mass *refMass);
    void draw();
    
};

#endif // SPRING_H
