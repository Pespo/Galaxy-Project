#include "Physics/ConstantSpring.hpp"

using namespace stein;

ConstantSpring::ConstantSpring(const Vector3f& force):
        m_force(force) {
    }

void ConstantSpring::generateForces(Particle* p1, Particle* p2) {
        p1->m_force += m_force;
    }