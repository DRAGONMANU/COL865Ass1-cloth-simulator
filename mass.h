#ifndef Mass_H
#define Mass_H
#include <vector>
#include "common.hpp"
#include "spring.h"

class Spring;

// A point mass is a mass at a position
// This object controls motion of the mass under the force of gravity
// and connected springs
class Mass
{
public:
    Mass(float mass, float x, float y, float z);

    void setPosition(float x, float y, float z);
    void addSpring(Spring *s);

    vec3 calculateForces();  // calculate all forces acting upon the mass
                                  // (gravity and springs
    void update(float dt);
    void draw();

    float mass;     // in kilograms;
    float dscale;   // distance scale in units/metre
    vec3 position;
    vec3 velocity;
    vec3 gravity;
    bool isFixedPosition;    // masses in fixed position do not move

    std::vector<Spring*> springs;  // holds all springs connected to a mass

};

#endif // Mass_H
