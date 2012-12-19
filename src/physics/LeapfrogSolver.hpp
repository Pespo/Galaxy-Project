#pragma once

#include "Physics/Spring.hpp"
#include "Physics/Particle.hpp"

using namespace stein;

class LeapfrogSolver {
public:
	LeapfrogSolver();
    LeapfrogSolver(float step);
    ~LeapfrogSolver();

    void solve(Particle* p);
    void getNextState(const Particle& particle, float dt, Particle& next) const;

    const float _step; // Step used to calculate movements
};