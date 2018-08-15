#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include "mass.hpp"
#include "spring.hpp"

using namespace std;

class MassSpringSystem
{
public:
	vector<Mass*> masses;
    vector<Spring*> springs;
    float dscale;  // distance scale
	MassSpringSystem()
	{
		dscale = 10.0;
	}

	Mass* addMass(float mass, float x, float y, float z)
	{
		Mass *m = new Mass(mass, x, y, z);
	    m->dscale = dscale;
	    masses.push_back(m);
	    return m;
	}

    Spring* addSpring(float springConstant, float restLength, Mass *mass1, Mass *mass2)
    {
    	Spring *s = new Spring(springConstant, restLength, mass1, mass2);
	    s->dscale = dscale;
	    mass1->addSpring(s);
	    mass2->addSpring(s);
	    springs.push_back(s);
	    return s;
    }

    void setDistanceScale(float scale)
    {
    	dscale  = scale;
    	for(int i = 0;i<masses.size();i++)
    	{
    		masses[i]->dscale = scale;
    	}
    	for(int j = 0;j<springs.size();j++)
    	{
    		springs[j]->dscale = scale;
    	}
    }

    void update(float dt)
    {
    	for(int i = 0;i<masses.size();i++)
    	{
    		masses[i]->update(dt);
    	}
    }

    void draw() 
    {
	    for (int i=0; i<masses.size(); i++) 
	        masses[i]->draw();
		for (int j=0; j<springs.size(); j++) 
	        springs[j]->draw();
	}
};

#endif
