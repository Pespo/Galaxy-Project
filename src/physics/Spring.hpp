#pragma once

#include "Physics/Particle.hpp"
#include <stein/math/Vector3f.hpp>

using namespace stein;

class Spring {
public:
    virtual ~Spring(){}
    virtual void generateForces(Particle* p1, Particle* p2) = 0;
};