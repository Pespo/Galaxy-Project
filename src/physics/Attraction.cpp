#include "Physics/Attraction.hpp"
#include <stein/math/Vector3f.hpp>

using namespace stein;

Attraction::Attraction() : m_force(1) {}

Attraction::Attraction(float force) :  m_force(force) {}

void Attraction::generateForces(Particle *p1, Particle *p2) {
        Vector3f vector = (p2->m_position - p1->m_position);
        p1->m_position += Vector3f(vector.x/20, vector.y/20, vector.z/20);
}