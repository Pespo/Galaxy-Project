// Scene.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Scene.hpp"
#include "Tools.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <stdexcept>
#include <iostream>

namespace stein {

// Default constructor
Scene::Scene() :
    defaultColor(Color::RED), defaultTransformation(Matrix4f::identity()), defaultShaderID(0), defaultTextureID0(NA), defaultTextureID1(NA), defaultMaterialID(NA) {
    /*glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);*/
}

Scene::~Scene() {
    for (size_t i = 0; i < storedObjects.size(); ++i)
        delete storedObjects[i];
}

// Adds the object in the Objects library array, 
// after the last added object, and only if the array is not full, returns the index
Object& Scene::createObject(GLenum primitiveType) {
    const size_t size = storedObjects.size();
    if (size >= maxStoredObjects)
        throw std::runtime_error("maximum number of stored objects reached");
    storedObjects.push_back(new Object(size, primitiveType));
    return *storedObjects.back();
}

// Adds the index of stored object to draw in the drawnObjects array, 
// after the last added, and only if the array is not full
GLuint Scene::addObjectToDraw(GLuint indexStoredObject) {
    const size_t size = drawnObjects.size();
	std::cout << maxDrawnObjects << std::endl;
    if (size >= maxDrawnObjects)
        throw std::runtime_error("maximum number of drawn objects reached");
    drawnObjects.push_back(ObjectInstance(size, indexStoredObject, defaultShaderID, defaultTextureID0, defaultTextureID1, defaultTransformation, defaultColor, defaultMaterialID));
    return size;
}

// Sets the color for the drawn object of index indexDrawObject
void Scene::setDrawnObjectColor(GLuint indexDrawnObject, const Color &color) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].color = color;
}

// Sets the transformation matrix for the drawn object of index indexDrawObject
void Scene::setDrawnObjectModel(GLuint indexDrawnObject, const Matrix4f &model) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].transformation = model;
}

// Sets the material for the drawn object of index indexDrawnObject
void Scene::setDrawnObjectMaterialID(GLuint indexDrawnObject, GLuint materialID) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].materialId = materialID;
}

// Sets the if of the shader to use on the drawn object of index indexDrawObject
void Scene::setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].shaderId = shaderID;
}

//Sets the ID of the texture to use on the drawn object of index indexDrawnObject
void Scene::setDrawnObjectTextureID(GLuint indexDrawnObject, GLuint textureUnit, GLuint textureID) {
    if (textureUnit == 0) drawnObjects[indexDrawnObject].textureId0 = textureID;
    if (textureUnit == 1) drawnObjects[indexDrawnObject].textureId0 = textureID;
}

// Sets the light
void Scene::setDefaultColor(const Color &color) {
    defaultColor = color;
    for (size_t i = 0; i < drawnObjects.size(); ++i)
        drawnObjects[i].color = defaultColor;
}

// Sets default transformation matrix
void Scene::setDefaultModel(const Matrix4f &_defaultModel) {
    defaultTransformation = _defaultModel;
    for (size_t i = 0; i < drawnObjects.size(); ++i)
        drawnObjects[i].transformation = defaultTransformation;
}

void Scene::setDefaultMaterialID(GLuint id) {
    defaultMaterialID = id;
}

// Sets default shader ID
void Scene::setDefaultShaderID(GLuint id) {
    defaultShaderID = id;
}

//Sets the ID of the texture to use on the drawn object of index indexDrawnObject
void Scene::setDefaultTextureID(GLuint textureUnit, GLuint textureID) {
    if (textureUnit == 0) defaultTextureID0 = textureID;
    if (textureUnit == 1) defaultTextureID1 = textureID;
}

// Decides what will elements drawn after this call will look like
void Scene::setAppearance(const ObjectInstance &instance) {
    const size_t shaderId = instance.shaderId;
    glUseProgram(shaderId); // From now on, this shader will be used.

    // We use the specific values of model per object
    setMatricesInShader(shaderId, instance.transformation, pCamera->getView(), pCamera->getPosition(), pCamera->getProjection());
    glUniform4fv(glGetUniformLocation(instance.shaderId, "color"), 1, instance.color);

    if(instance.materialId != NA) {
        // Sets the material
		setMaterialInShader(shaderId, instance.materialId);
    }
    
    /*if(instance.lightId != NA) {
        // Sets the light
        setLightInShader(shaderId, lights[instance.lightId]);
    }*/

    if(instance.textureId0 != NA) {
        //Selects our current texture for unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, instance.textureId0);
        if(instance.textureId1 != NA) {
            //Selects our current texture for unit 1
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, instance.textureId1);
        }
        setTextureUnitsInShader(shaderId);
    }

    // Specifies which VBO were filled
    const Object * pObject = storedObjects[instance.objectId];
    assert(pObject);
    setFilledDataInShader(shaderId, pObject->hasPrimitives(), pObject->hasNormals(), pObject->hasUvs(), pObject->hasColors());
}

// Draw all Objects
void Scene::drawObjectsOfScene() {
    for (size_t i = 0; i < drawnObjects.size(); ++i) {
        const ObjectInstance &instance = drawnObjects[i];
        setAppearance(instance);
        storedObjects[instance.objectId]->drawObject();
    }
}

} // namespace stein
