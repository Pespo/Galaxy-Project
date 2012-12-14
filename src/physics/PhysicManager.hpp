#pragma once

#include "Physics/Particle.hpp"
#include "Physics/LeapfrogSolver.hpp"
#include "Physics/GravitySpring.hpp"
#include "Physics/HookSpring.hpp"
#include <stein/math/Vector3f.hpp>
#include <stein/GLHeaders.hpp>
#include <vector>

class PhysicManager {
public:
    PhysicManager();
    ~PhysicManager();

    GLuint addPhysicToObject(GLuint indexStoredObject);

    void setPhysicObjectPosition(GLuint indexPhysicalObject, const Vector3f pos);
    void setPhysicObjectMass(GLuint indexPhysicalObject, const float mass);
    void setPhysicObjectVelocity(GLuint indexPhysicalObject, const Vector3f velocity);
    void setPhysicObjectForce(GLuint indexPhysicalObject, const Vector3f force);

    void setDefaultPosition(const Vector3f pos);
    void setDefaultMass(const float mass);
    void setFefaultVelocity(const Vector3f velocity);
    void setDefaultForce(const Vector3f force);

    void solve();
    void applySpring();

    const static size_t maxPhysicalObjects = 200; // An initial limit of drawable objects
    std::vector<Particle*> physicalObjects;

    GravitySpring gSpring;
    LeapfrogSolver solver; // Calculates objects movements
private:
    
    float defaultMass;
    Vector3f defaultPosition;
    Vector3f defaultVelocity;
    Vector3f defaultForce;
};