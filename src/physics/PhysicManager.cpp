#include "Physics/PhysicManager.hpp"
#include "physics/Particle.hpp"
#include "Physics/LeapfrogSolver.hpp"
#include <stein/Scene.hpp>

PhysicManager::PhysicManager() : defaultPosition(0.0, 0.0, 0.0), defaultMass(1.0), defaultVelocity(0.0, 0.0, 0.0), defaultForce(0.0, 0.0, 0.0), defaultRotation(0.0, 0.0, 0.0), solver(0.02) {};

PhysicManager::~PhysicManager() {}

GLuint PhysicManager::addPhysicToObject(GLuint indexStoredObject) {
    const size_t size = physicalObjects.size();
    if (size >= maxPhysicalObjects)
        throw std::runtime_error("maximum number of drawn particles reached");
    physicalObjects.push_back(new Particle(indexStoredObject, defaultPosition, defaultMass, defaultVelocity, defaultForce, defaultRotation));
    return size;
}

void PhysicManager::setPhysicObjectPosition(GLuint indexPhysicalObject, const Vector3f pos) {
	assert(indexPhysicalObject<physicalObjects.size());
    physicalObjects[indexPhysicalObject]->m_position = pos;
}

void PhysicManager::setPhysicObjectMass(GLuint indexPhysicalObject, const float mass) {
	assert(indexPhysicalObject<physicalObjects.size());
    physicalObjects[indexPhysicalObject]->m_mass = mass;
}

void PhysicManager::setPhysicObjectVelocity(GLuint indexPhysicalObject, const Vector3f velocity) {
	assert(indexPhysicalObject<physicalObjects.size());
    physicalObjects[indexPhysicalObject]->m_velocity = velocity;
}

void PhysicManager::setPhysicObjectForce(GLuint indexPhysicalObject, const Vector3f force) {
	assert(indexPhysicalObject<physicalObjects.size());
    physicalObjects[indexPhysicalObject]->m_force = force;
}

void PhysicManager::setPhysicObjectRotation(GLuint indexPhysicalObject, const Vector3f rotation){
    assert(indexPhysicalObject<physicalObjects.size());
    physicalObjects[indexPhysicalObject]->m_rotation = rotation;

};

void PhysicManager::setDefaultPosition(const Vector3f pos) {
	defaultPosition = pos;
    for (size_t i = 0; i < physicalObjects.size(); ++i)
        physicalObjects[i]->m_position = defaultPosition;
}

void PhysicManager::setDefaultMass(const float mass) {
	defaultMass = mass;
    for (size_t i = 0; i < physicalObjects.size(); ++i)
        physicalObjects[i]->m_mass = defaultMass;
}

void PhysicManager::setDefaultVelocity(const Vector3f velocity) {
	defaultVelocity = velocity;
    for (size_t i = 0; i < physicalObjects.size(); ++i)
        physicalObjects[i]->m_velocity = defaultVelocity;
}

void PhysicManager::setDefaultForce(const Vector3f force) {
	defaultForce = force;
    for (size_t i = 0; i < physicalObjects.size(); ++i)
        physicalObjects[i]->m_force = defaultForce;
}

void PhysicManager::setDefaultRotation(const Vector3f rotation){
    defaultRotation = rotation;
    for (size_t i = 0; i < physicalObjects.size(); ++i)
        physicalObjects[i]->m_force = defaultForce;
}

void PhysicManager::solve(){
	for (size_t i = 0; i < physicalObjects.size(); ++i)
	        solver.solve(physicalObjects[i]);
}

void PhysicManager::applySprings(){
	for(int i=1; i < physicalObjects.size(); ++i) {
        //attraction.generateForces(physicalObjects[i], physicalObjects[i-1]);
        for(int j=i+1; j < physicalObjects.size(); ++j)
		  hookSpring.generateForces(physicalObjects[i], physicalObjects[j]);
	}
}