#pragma once

#include "ShaderGLSL.hpp"
#include "GLCommon.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "LinearAlgebra.hpp"

#include <vector>

// Forward declaration
struct Object;

// A container for objects
class Scene {
    struct ObjectInstance {
        size_t objectId;
        size_t shaderId;
        float* transformation;
        Color color;

        ObjectInstance(size_t objectId, size_t shaderId, const float* transformation1, const Color& color) :
            objectId(objectId), shaderId(shaderId), color(color) {
            assert(sizeof(transformation1) / sizeof(float) == 16);
			mat4fCopy(transformation, transformation1);
        }
    };

    struct GUIStates {
        bool panLock;
		bool turnLock;
        bool zoomLock;
        int lockPositionX;
        int lockPositionY;
        int camera;
        double time;
        bool playing;
        const float PAN_SPEED;
        const float ZOOM_SPEED;
        const float MOUSE_TURN_SPEED;

        GUIStates() : PAN_SPEED(0.005f), ZOOM_SPEED(0.005f), MOUSE_TURN_SPEED(0.005f) {}
    };

    /*float PAN_SPEED = 0.005f;
    float ZOOM_SPEED = 0.005f;
    float MOUSE_TURN_SPEED = 0.005f;*/
 
	GLuint* drawnObjectsTexture0IDs;
	GLuint* drawnObjectsTexture1IDs;    
	GLuint nbDrawnObjects;

    std::vector<Object*> storedObjects; // Library of Objects to use from GPU
    std::vector<ObjectInstance> drawnObjects;
    //std::vector<Light*> storedLights;
    //std::vector<LightInstance> drawnLights;

    Color defaultColor; // Default color for drawn elements
    float* defaultTransformation; // Default transformation matrix for drawn elements
    GLfloat lightPosition[4]; // Position of the light used in shader
    GLfloat lightPower; // Power of the light used in shader
    
    
public:
    enum Shader {

    };

    enum Texture {
        
    };

    const static size_t maxStoredObjects = 500; // An initial limit of storable objects
    const static size_t maxDrawnObjects = 200; // An initial limit of drawable objects  
    static GLuint defaultShaderID; // Default shaderID for drawn elements

    Scene();
    ~Scene();

    Camera* pCamera; // Camera used to watch the scene
    GUIStates guiStates; // Useful camera states

    void init_gui_states(GUIStates & guiStates);

    GLuint loadTexture(const char* fileName, int comp);
    void loadShader(ShaderGLSL & shader,  const char * path);

    Object& createObject(GLenum primitiveType);
    GLuint addObjectToDraw(GLuint indexStoredObject, GLuint shaderID = defaultShaderID);

    void setDrawnObjectColor(GLuint indexDrawnObject, const Color &color);
    void setDrawnObjectModel(GLuint indexDrawnObject, float model[16]);
    void setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID);
    void setDrawnObjectTextureID(GLuint indexDrawnObject, GLuint textureUnit, GLuint textureID);

    void setDefaultTextureID(GLuint defaultShaderID);
    void setDefaultShaderID(GLuint defaultShaderID);
    void Scene::setDefaultColor(const Color &color);
    void setDefaultModel(const float* defaultModel);

    void setLight(GLfloat * position, GLfloat power);

    void drawObjectsOfScene();

    void updateCamera(int mousex, int mousey, int zButton, int qButton, int sButton, int dButton);

private:
    //void setAppearance(const ObjectInstance &);
};