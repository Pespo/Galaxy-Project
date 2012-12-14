#include "Physics/GravitySpring.hpp"

GravitySpring::GravitySpring() {}
GravitySpring::GravitySpring(Vector3f force) : m_force(force) {}
GravitySpring::~GravitySpring(){}

void GravitySpring::generateForces(Particle* p1, Particle* p2) {
    p1->m_force += m_force*p1->m_mass;
}