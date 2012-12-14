#pragma once

#include "Physics/Spring.hpp"
#include "Physics/Particle.hpp"
#include <stein/math/Vector3f.hpp>

using namespace stein;

class ConstantSpring: public Spring {
public:
    ConstantSpring(const Vector3f& force);
    void generateForces(Particle* p1, Particle* p2);
    
private:
    Vector3f m_force;
};