#pragma once

#include "Physics/Particle.hpp"
#include "Physics/LeapfrogSolver.hpp"
#include "Physics/GravitySpring.hpp"
#include "Physics/cineticBrake.hpp"
#include "Physics/HookSpring.hpp"
#include "Physics/Attraction.hpp"
#include <stein/math/Vector3f.hpp>
#include <stein/GLHeaders.hpp>
#include <vector>

class PhysicManager {
public:
    PhysicManager();
    PhysicManager(float step);
    ~PhysicManager();

    GLuint addPhysicToObject(GLuint indexStoredObject);

    void setPhysicObjectPosition(GLuint indexPhysicalObject, const Vector3f pos);
    void setPhysicObjectMass(GLuint indexPhysicalObject, const float mass);
    void setPhysicObjectVelocity(GLuint indexPhysicalObject, const Vector3f velocity);
    void setPhysicObjectForce(GLuint indexPhysicalObject, const Vector3f force);
    void setPhysicObjectRotation(GLuint indexPhysicalObject, const Vector3f rotation);

    void setHookSpring(float k, float freeLength);
    void setAttraction(float force);
    void setCineticBrake(float z);

    //void setStep(float step);
    void setDefaultPosition(const Vector3f pos);
    void setDefaultMass(const float mass);
    void setDefaultVelocity(const Vector3f velocity);
    void setDefaultForce(const Vector3f force);
    void setDefaultRotation(const Vector3f rotation);
    
    void solve();
    void applySprings();

    const static size_t maxPhysicalObjects = 500; // An initial limit of drawable objects
    std::vector<Particle*> physicalObjects;

    bool bHookSpring;
    bool bAttraction;
    bool bCineticBrake;

    HookSpring hookSpring;
    Attraction attraction;
    CineticBrake cineticBrake;
    LeapfrogSolver solver; // Calculates objects movements

private:
    float step;
    float defaultMass;
    Vector3f defaultPosition;
    Vector3f defaultVelocity;
    Vector3f defaultForce;
    Vector3f defaultRotation;
};