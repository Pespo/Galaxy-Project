// Scene.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "GLHeaders.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "Light.hpp"

#include <vector>

namespace stein {

// Forward declaration
struct Object;

// A container for objects
class Scene {
private:
    struct ObjectInstance {
        size_t id;
        size_t objectId;
        size_t shaderId;
        GLuint textureId0;
        GLuint textureId1;
        Matrix4f transformation;
        Color color;
        GLuint materialId;

        ObjectInstance(size_t id, size_t objectId, size_t shaderId, GLuint textureId0, GLuint textureId1, const Matrix4f& transformation, const Color&color, GLuint materialId) :
            id(id), objectId(objectId), shaderId(shaderId), textureId0(textureId0), textureId1(textureId1), transformation(transformation), color(color), materialId(materialId) {
        }
    };

    std::vector<Object*> storedObjects; // Library of Objects to use from GPU
    std::vector<ObjectInstance> drawnObjects;
    

    Color defaultColor; // Default color for drawn elements
    Matrix4f defaultTransformation; // Default transformation matrix for drawn elements
    GLuint defaultShaderID; // Default shaderID for drawn elements
    GLuint defaultTextureID0; // Default textureID for drawn elements
    GLuint defaultTextureID1;
    GLuint defaultMaterialID; // Default material for drawn elements

    const static unsigned int NA = -1;

    void setAppearance(const ObjectInstance &);

public:
    const static size_t maxStoredObjects = 50; // An initial limit of storable objects
    const static size_t maxDrawnObjects = 500; // An initial limit of drawable objects

    Scene();
    ~Scene();

    Camera* pCamera; // Camera used to watch the scene

    Object& createObject(GLenum primitiveType);
    GLuint addObjectToDraw(GLuint indexStoredObject);

    void setDrawnObjectColor(GLuint indexDrawnObject, const Color &color);
    void setDrawnObjectModel(GLuint indexDrawnObject, const Matrix4f &model);
    void setDrawnObjectMaterialID(GLuint indexDrawnObject, GLuint materialID);
    void setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID);
    void setDrawnObjectTextureID(GLuint indexDrawnObject, GLuint textureUnit, GLuint textureID);

    void setDefaultColor(const Color &defaultColor);
    void setDefaultModel(const Matrix4f &defaultModel);
    void setDefaultMaterialID(GLuint defaultMaterialID);
    void setDefaultShaderID(GLuint defaultShaderID);
    void setDefaultTextureID(GLuint textureUnit, GLuint defaultTextureID);

    void drawObjectsOfScene();
};

} // namespace stein

#endif // __SCENE_HPP__
