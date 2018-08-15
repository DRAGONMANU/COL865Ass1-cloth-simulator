#ifndef MASS_HPP
#define MASS_HPP

#include "common.hpp"
#include "mass.h"

Mass::Mass(float m, float x, float y, float z)
{
    mass = m;
    dscale = 0.1;
    position = vec3(x, y, z);
    velocity = vec3(0.0, 0.0, 0.0);
    gravity = vec3(0.0, -9.81, 0.0);
}
void Mass::setPosition(float x, float y, float z)
{
    position = vec3(x,y,z);
}

void Mass::addSpring(Spring *s)
{
    springs.push_back(s);
}

vec3 Mass::calculateForces()  // calculate all forces acting upon the mass
{
    // vec3 fg = gravity * mass;
    vec3 fs = vec3(0.0, 0.0, 0.0);

    for (int i=0; i<springs.size(); i++) 
        fs += springs[i]->getForce(this);

    // vec3 force = fg + fs;
    vec3 force = fs;
    return force;
}

void Mass::update(float dt)
{
    if (mass==0) 
        return;

    /*Symplectic Euler*/
    // F = m*a -> a = F / m
    vec3 acc = calculateForces() / mass;
    // vf = vi + a*dt
    velocity = velocity + acc * dt;
    // pf = pi + vf*dt
    position = position + velocity * dscale * dt;



}

void Mass::draw()
{
    drawPoint(position);
}

#endif
