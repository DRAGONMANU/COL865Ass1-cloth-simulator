#ifndef SPRING_HPP
#define SPRING_HPP

#include "common.hpp"

class Mass;

class Spring
{
public:
	float k;         // spring constant
    float kd;        // damping constants
    float rl;		 // rest length
    Mass *m1;   // mass1 on spring ends
    Mass *m2;	 // mass2 on spring ends
    float dscale;
	Spring(float springConstant, float restLength, Mass *mass1, Mass *mass2)
	{
		k = springConstant;
		kd = 0.1;
		dscale = 10;
		rl = restLength;
		m1 = mass1;
		m2 = mass2;
	}

    vec3 getForce(Mass *refMass)
    {
    	if (!(refMass == m1 or refMass == m2))
        	return vec3(0, 0, 0);
   
	    // spring force
	    //TODO
	    float dist = (m1->position - m2->position).norm();
	    float scalar = dscale*k*(dist - rl);
	    vec3 dir = (m2->position - m1->position).normalized();

	    // find speed of contraction/expansion for damping force
	    float s1 = m1->velocity.dot(dir);
	    float s2 = m2->velocity.dot(dir);
	    float dampingScalar = -kd * (s1 + s2);

	    if (refMass == m1) {
	        return (scalar + dampingScalar) * dir;
	    } else{
	        return (-scalar + dampingScalar) * dir;
	    }

    }
    
    void draw()
    {
    	drawLine(m1->position,m2->position);
    }
};

#endif
