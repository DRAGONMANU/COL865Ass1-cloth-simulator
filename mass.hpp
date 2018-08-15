#ifndef MASS_HPP
#define MASS_HPP

#include "common.hpp"
#include "mass.h"

vec3 GRAVITY = vec3(0.0, -9.81, 0.0);

Mass::Mass(float m, float x, float y, float z)
{
    mass = m;
    position = vec3(x, y, z);
    velocity = vec3(0.0, 0.0, 0.0);
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
    vec3 fg = GRAVITY * mass;
    vec3 fs = vec3(0.0, 0.0, 0.0);

    for (int i=0; i<springs.size(); i++) 
        fs += springs[i]->getForce(this);

    vec3 force = fg + fs;
    return force;
}

void Mass::update(float dt, int choice)
{
    if (mass==0) 
        return;

    if(choice==0) // Symplectic Euler
    {
        vec3 acc = calculateForces() / mass;
        velocity = velocity + acc * dt;
        position = position + velocity * dt;
    }
    else if (choice==1) // Midpoint
    {
        vec3 init_pos = position;
        vec3 init_vel = velocity;
        velocity = velocity + calculateForces() / mass * dt/2;
        position = position + velocity * dt/2;        
        vec3 v_half = velocity;
        velocity = init_vel + calculateForces() / mass * dt;
        position = init_pos + v_half * dt;
    }
    else if (choice==2) // RK4
    {
        //TODO
        vec3 k1,k2,k3,k4,l1,l2,l3,l4;
        vec3 init_pos = position;
        vec3 init_vel = velocity;

        l1 = calculateForces() / mass;
        k1 = velocity;

        position = init_pos + k1 * dt/2;
        velocity = init_vel + l1 * dt/2;
        l2 = calculateForces() / mass;
        k2 = velocity;

        position = init_pos + k2 * dt/2;
        velocity = init_vel + l2 * dt/2;
        l3 = calculateForces() / mass;
        k3 = velocity;

        position = init_pos + k3 * dt;
        velocity = init_vel + l3 * dt;
        l4 = calculateForces() / mass;
        k4 = velocity;

        velocity = init_vel + (1/6*(l1)+1/3*(l2)+1/3*(l3)+1/6*(l4)) * dt;
        position = init_pos + (1/6*(k1)+1/3*(k2)+1/3*(k3)+1/6*(k4)) * dt;   
    }
    else
    {
        exit(0);
    }
}
#endif
