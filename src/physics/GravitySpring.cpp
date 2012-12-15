#include "Physics/GravitySpring.hpp"

GravitySpring::GravitySpring() : m_force(Vector3f(0., -1., 0.)) {} 
GravitySpring::GravitySpring(Vector3f force = Vector3f(0., -1., 0.)) : m_force(force) {}
GravitySpring::~GravitySpring(){}

void GravitySpring::generateForces(Particle* p1, Particle* p2) {
    p1->m_force += m_force*p1->m_mass;
}