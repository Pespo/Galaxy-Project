#pragma once

#include <stein/math/Vector3f.hpp>

using namespace stein;

class Particle {
public:
	const size_t m_ObjectInstanceId;
	Vector3f m_position;
    float m_mass;
    Vector3f m_velocity;
    Vector3f m_force;

	Particle(size_t ObjectInstanceId, Vector3f position, float mass, Vector3f velo, Vector3f force) : m_ObjectInstanceId(ObjectInstanceId), m_position(position), m_mass(mass), m_velocity(velo), m_force(force) {}
	~Particle(){}
};