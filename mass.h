#ifndef Mass_H
#define Mass_H
#include <vector>
#include "common.hpp"
#include "spring.h"

class Spring;

class Mass
{
public:
    float mass;
    vec3 position;
    vec3 velocity;

    std::vector<Spring*> springs;
    Mass(float mass, float x, float y, float z);

    void setPosition(float x, float y, float z);
    void addSpring(Spring *s);

    vec3 calculateForces();
    void update(float dt, int choice);
};

#endif
