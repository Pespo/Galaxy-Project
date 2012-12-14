#pragma once

#include "Physics/Particle.hpp"
#include "Physics/Spring.hpp"

using namespace stein;

class Attraction : public Spring {
public:
    Attraction(float force);
    virtual void generateForces(Particle *p1, Particle *p2);

    float m_force;
};