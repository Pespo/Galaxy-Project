#include "Physics/CineticBrake.hpp"
#include <stein/math/Vector3f.hpp>

using namespace stein;

CineticBrake::CineticBrake(float dt, float z = 0.05) : m_dt(dt),  m_z(z) {}

void CineticBrake::generateForces(Particle *p1, Particle *p2) {
	Vector3f v = p2->m_velocity - p1->m_velocity;
	p1->m_force += v * (m_z / m_dt);
	p2->m_force -= v * (m_z / m_dt);
}