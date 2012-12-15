#include "Physics/HookSpring.hpp"
#include <stein/math/Vector3f.hpp>
#include <cmath>

using namespace std;
using namespace stein;

HookSpring::HookSpring() : m_k(1), m_freeLength(0.4), m_epsilon(0.000001), m_inversion(false) {}

HookSpring::HookSpring(float k = 1, float freeLength = 0.4, float epsilon = 0.000001, bool inversion = false) : m_k(k), m_freeLength(freeLength), m_epsilon(epsilon), m_inversion(inversion) {}

HookSpring::~HookSpring(){}

void HookSpring::generateForces(Particle *p1, Particle *p2) {
     Vector3f vector = (p2->m_position - p1->m_position);
     float d = sqrt(vector.x*vector.x + vector.y*vector.y);
     float e = 1-(m_freeLength/max(m_epsilon,d));
     if(!m_inversion){
         p1->m_force += vector*m_k*e/*((float)glm::sqrt((p2->m_color.x - p1->m_color.x)*(p2->m_color.x - p1->m_color.x) + (p2->m_color.y - p1->m_color.y) * (p2->m_color.y - p1->m_color.y) + (p2->m_color.z - p1->m_color.z) * (p2->m_color.z - p1->m_color.z)))*/;
         p2->m_force -= vector*m_k*e/*((float)glm::sqrt((p2->m_color.x - p1->m_color.x)*(p2->m_color.x - p1->m_color.x) + (p2->m_color.y - p1->m_color.y) * (p2->m_color.y - p1->m_color.y) + (p2->m_color.z - p1->m_color.z) * (p2->m_color.z - p1->m_color.z)))*/;
     } else {
         p1->m_force -= vector*m_k*e/*(p2->m_mass*10)*/;
         p2->m_force += vector*m_k*e/*(p1->m_mass*10)*/;
     }
 }