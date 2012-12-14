#pragma once

#include "Physics/Spring.hpp"
#include "Physics/Particle.hpp"
#include <stein/math/Vector3f.hpp>

using namespace stein;

class GravitySpring : public Spring {
public:
    GravitySpring();
    GravitySpring(Vector3f force);
    ~GravitySpring();

    void generateForces(Particle* p1, Particle* p2);
    
    Vector3f m_force;
};