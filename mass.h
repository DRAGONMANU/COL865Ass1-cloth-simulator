#ifndef Mass_H
#define Mass_H
#include <vector>
#include "common.hpp"
#include "spring.h"

class Spring;

class Mass
{
public:
    float mass;     // in kilograms;
    vec3 position;
    vec3 velocity;
    vec3 gravity;
    bool isFixedPosition;    // masses in fixed position do not move

    std::vector<Spring*> springs;  // holds all springs connected to a mass
    Mass(float mass, float x, float y, float z);

    void setPosition(float x, float y, float z);
    void addSpring(Spring *s);

    vec3 calculateForces();
    void update(float dt, int choice);
    void draw();    
};

#endif
