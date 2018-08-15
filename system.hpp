#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include "spring.hpp"
#include "mass.hpp"

using namespace std;

class MassSpringSystem
{
public:
	vector<Mass*> masses;
    vector<Spring*> springs;
	MassSpringSystem()
	{
	}

	Mass* addMass(float mass, float x, float y, float z)
	{
		Mass *m = new Mass(mass, x, y, z);
	    masses.push_back(m);
	    return m;
	}

    Spring* addSpring(float springConstant, float restLength, Mass *mass1, Mass *mass2)
    {
    	Spring *s = new Spring(springConstant, restLength, mass1, mass2);
	    mass1->addSpring(s);
	    mass2->addSpring(s);
	    springs.push_back(s);
	    return s;
    }

    void update(float dt,int choice)
    {
    	for(int i = 0;i<masses.size();i++)
    	{
    		masses[i]->update(dt,choice);
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
