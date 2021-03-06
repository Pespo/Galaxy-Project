#include "Physics/HookSpring.hpp"
#include <stein/math/Vector3f.hpp>
#include <cmath>

using namespace std;
using namespace stein;

HookSpring::HookSpring() : m_k(2), m_freeLength(0.1), m_epsilon(0.000001) {}

HookSpring::HookSpring(float k = 1, float freeLength = 0.4, float epsilon = 0.000001) : m_k(k), m_freeLength(freeLength), m_epsilon(epsilon) {}

HookSpring::~HookSpring(){}

void HookSpring::generateForces(Particle *p1, Particle *p2) {
    Vector3f vector = (p2->m_position - p1->m_position);
    float d = sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
    float e = 1-(m_freeLength/max(m_epsilon,d));

    p1->m_force += vector*m_k*e/*((float)glm::sqrt((p2->m_color.x - p1->m_color.x)*(p2->m_color.x - p1->m_color.x) + (p2->m_color.y - p1->m_color.y) * (p2->m_color.y - p1->m_color.y) + (p2->m_color.z - p1->m_color.z) * (p2->m_color.z - p1->m_color.z)))*/;
    p2->m_force -= vector*m_k*e/*((float)glm::sqrt((p2->m_color.x - p1->m_color.x)*(p2->m_color.x - p1->m_color.x) + (p2->m_color.y - p1->m_color.y) * (p2->m_color.y - p1->m_color.y) + (p2->m_color.z - p1->m_color.z) * (p2->m_color.z - p1->m_color.z)))*/;
 }