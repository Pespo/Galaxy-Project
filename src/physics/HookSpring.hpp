#pragma once

#include "Physics/Particle.hpp"
#include "Physics/Spring.hpp"

using namespace stein;

class HookSpring: public Spring {

public:
	HookSpring();
	HookSpring(float k, float freeLength, float epsilon);
	~HookSpring();

	virtual void generateForces(Particle *p1, Particle *p2);

	float m_k;
	float m_freeLength;
	float m_epsilon;
};