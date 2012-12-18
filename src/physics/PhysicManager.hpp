#pragma once

#include "Physics/Particle.hpp"
#include "Physics/LeapfrogSolver.hpp"
#include "Physics/GravitySpring.hpp"
#include "Physics/HookSpring.hpp"
#include "Physics/Attraction.hpp"
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
    void setPhysicObjectRotation(GLuint indexPhysicalObject, const Vector3f rotation);

    void setDefaultPosition(const Vector3f pos);
    void setDefaultMass(const float mass);
    void setDefaultVelocity(const Vector3f velocity);
    void setDefaultForce(const Vector3f force);
    void setDefaultRotation(const Vector3f rotation);
    
    void solve();
    void applySprings();

    const static size_t maxPhysicalObjects = 200; // An initial limit of drawable objects
    std::vector<Particle*> physicalObjects;

    HookSpring hookSpring;
    Attraction attraction;
    LeapfrogSolver solver; // Calculates objects movements

private:
    float defaultMass;
    Vector3f defaultPosition;
    Vector3f defaultVelocity;
    Vector3f defaultForce;
    Vector3f defaultRotation;
};