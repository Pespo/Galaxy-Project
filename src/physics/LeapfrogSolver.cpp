#include "Physics/LeapfrogSolver.hpp"
#include <stein/math/Vector3f.hpp>
#include <iostream>

using namespace stein;

LeapfrogSolver::LeapfrogSolver(float step = 0.002) : _step(step){}
LeapfrogSolver::~LeapfrogSolver(){}

void LeapfrogSolver::solve(Particle* p) {
    //std::cout << p.force.x << "," << p.force.y << std::endl;
    p->m_velocity += (p->m_force/p->m_mass)*_step;
    p->m_position += p->m_velocity*_step;
    p->m_force = Vector3f(0,0,0);
}

void LeapfrogSolver::getNextState(const Particle& particle, float dt, Particle& next) const {
    next.m_velocity = (particle.m_force / particle.m_mass).crossP(particle.m_velocity + dt);
    next.m_position = next.m_velocity.crossP(particle.m_position + dt);
}