#include "Physics/CineticBrake.hpp"
#include <stein/math/Vector3f.hpp>

using namespace stein;

CineticBrake::CineticBrake() : m_step(0.02),  m_z(0.00001) {}

CineticBrake::CineticBrake(float dt, float z = 0.00001) : m_step(dt),  m_z(z) {}

void CineticBrake::generateForces(Particle *p1, Particle *p2) {
	Vector3f v = p2->m_velocity - p1->m_velocity;
	p1->m_force += v * (m_z / m_step);
	p2->m_force -= v * (m_z / m_step);
}