#pragma once

#include "Physics/Particle.hpp"
#include "Physics/Spring.hpp"

using namespace stein;

class CineticBrake: public Spring {
public:
	CineticBrake();
   	CineticBrake(float dt, float z);
   virtual void generateForces(Particle *p1, Particle *p2);

private:
    float m_dt;
    float m_z;
};