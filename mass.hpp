#ifndef MASS_HPP
#define MASS_HPP

#include "common.hpp"
#include "spring.hpp"


class Mass
{
public:
    float mass;     // in kilograms;
    float dscale;   // distance scale in units/metre
    vec3 position;
    vec3 velocity;
    vec3 gravity;
    bool isFixedPosition;    // masses in fixed position do not move
    std::vector<Spring*> springs;  // holds all springs connected to a mass
    
    Mass(float m, float x, float y, float z)
    {
        mass = m;
        dscale = 0.1;
        position = vec3(x, y, z);
        velocity = vec3(0.0, 0.0, 0.0);
        gravity = vec3(0.0, -9.81, 0.0);
        isFixedPosition = false;
    }
    void setPosition(float x, float y, float z)
    {
        position = vec3(x,y,z);
    }

    void addSpring(Spring *s)
    {
        springs.push_back(s);
    }

    vec3 calculateForces()  // calculate all forces acting upon the mass
    {
        vec3 fg = gravity * mass;
        vec3 fs = vec3(0.0, 0.0, 0.0);

        for (int i=0; i<springs.size(); i++) 
            fs += springs[i]->getForce(this);

        vec3 force = fg + fs;
        return force;
    }

    void update(float dt)
    {
        if (isFixedPosition) 
            return;


        /*Symplectic Euler*/
        // F = m*a -> a = F / m
        vec3 acc = calculateForces() / mass;
        // vf = vi + a*dt
        velocity = velocity + acc * dt;
        // pf = pi + vf*dt
        position = position + velocity * dscale * dt;
    }

    void draw()
    {
        glColor4f(0.0, 0.5, 1.0, 1.0);
        glPointSize(2.0);
        glBegin(GL_POINTS);
        glVertex3f(position.x, position.y, position.z);
        glEnd();
    }

    
};

#endif
