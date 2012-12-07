#include "Scene.hpp"
#include "LinearAlgebra.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Color.hpp"
#include "Transform.hpp"

#include "stb_image/stb_image.h"
#include "GL/glfw.h"
#include <stdexcept>
#include <exception>
#include <iostream>

GLuint Scene::defaultShaderID = 0;

// Default constructor
Scene::Scene() : defaultColor(Color::WHITE), pCamera(NULL) {

    mat4fToIdentity(defaultTransformation);
    // Light creation
    GLfloat lightPosition[] = { 0.0, 5.0, -5.0, 1.0 };
    GLfloat lightPower = 1.0;
    setLight(lightPosition, lightPower);
    
    try {
		drawnObjectsTexture0IDs = new GLuint[maxDrawnObjects]();
		drawnObjectsTexture1IDs = new GLuint[maxDrawnObjects]();
		for (size_t i = 0; i < maxDrawnObjects; i++) {
			drawnObjectsTexture0IDs[i] = 0;
			drawnObjectsTexture1IDs[i] = 0;
		}
	} catch(...) {
		// @TODO : Find better exception
		std::cerr << "BUFFER OVERFLOW !!" << std::endl;
	}

	glEnable(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

}

Scene::~Scene() {
    for (size_t i = 0; i < storedObjects.size(); ++i)
        delete storedObjects[i];
	delete [] drawnObjectsTexture0IDs;
	delete [] drawnObjectsTexture1IDs;
}

void Scene::init_gui_states(GUIStates & guiStates) {
    guiStates.panLock = false;
    guiStates.zoomLock = false;
    guiStates.lockPositionX = 0;
    guiStates.lockPositionY = 0;
    guiStates.camera = 0;
    guiStates.time = 0.0;
    guiStates.playing = false;
}

GLuint Scene::loadTexture(const char* fileName, int comp) {
    int w;
    int h;
    int c = comp;
    unsigned char *data;

    try {
        // Loads the image from a ppm file to an unsigned char array
        data = stbi_load(fileName, &w, &h, &c, c);
     }
     catch (...) {
         return 0;
    }   
    //Selects our current unit texture
    glActiveTexture(GL_TEXTURE0);

    // Allocates a texture id
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    // Selects our current texture
    glBindTexture(GL_TEXTURE_2D, textureID);


    // How to handle not normalised uvs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // How to handle interpolation from texels to fragments
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Specifies which image will be used for this texture objet
    switch(comp) {
        case 1 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            break;
        case 2 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, w, h, 0, GL_RG, GL_UNSIGNED_BYTE, data);
            break;
        case 3 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
    }

    fprintf(stderr, "Texture %dx%d:%d\n", w, h, c);
    
    return textureID;
};

void Scene::loadShader(ShaderGLSL & shader,  const char * path) {

    int status = load_shader_from_file(shader, path, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
    if ( status == -1 )
    {
        std::cerr << "Error on loading " << path << std::endl;
        //exit( EXIT_FAILURE );
    }

}

void Scene::updateCamera(int mousex, int mousey, int zButton, int qButton, int sButton, int dButton) {
	if(zButton == GLFW_PRESS || sButton == GLFW_PRESS)
        guiStates.zoomLock = true;
    else
        guiStates.zoomLock = false;

    if( qButton == GLFW_PRESS || dButton == GLFW_PRESS )
        guiStates.panLock = true;
    else
        guiStates.panLock = false;

	int diffLockPositionX = mousex - guiStates.lockPositionX;
	int diffLockPositionY = mousey - guiStates.lockPositionY;
	if (guiStates.turnLock)
	{
	    pCamera->turn(diffLockPositionY * guiStates.MOUSE_TURN_SPEED,
	                diffLockPositionX * guiStates.MOUSE_TURN_SPEED);

	}
	if (guiStates.zoomLock)
	{
	    float panZ = 0.0;
	    if (zButton == GLFW_PRESS)
	        panZ = 1.f;
	    else if (sButton == GLFW_PRESS)
	        panZ = -1.f;
	    pCamera->pan(0, 0, panZ * guiStates.ZOOM_SPEED);
	}
	if (guiStates.panLock)
	{
	    float panX = 0.0;
	    if (dButton == GLFW_PRESS)
	        panX = 1.f;
	    else if (qButton == GLFW_PRESS)
	        panX = -1.f;
	    pCamera->pan(panX * guiStates.PAN_SPEED, 0, 0);
	}
	guiStates.lockPositionX = mousex;
	guiStates.lockPositionY = mousey;

	float projection[16];
    float worldToView[16];
    float objectToWorld[16];
    float cameraPosition[4];
    float orthoProj[16];
    ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0, orthoProj);
    mat4fCopy(projection, pCamera->perspectiveProjection());
    mat4fCopy(worldToView, pCamera->worldToView());
    mat4fToIdentity(objectToWorld);
    vec4fCopy(cameraPosition, pCamera->position());
    float viewProjection[16];     
    float iviewProjection[16];       
    mat4fMul( worldToView, projection, viewProjection);
    mat4fInverse(viewProjection, iviewProjection);
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
GLuint Scene::addObjectToDraw(GLuint indexStoredObject, GLuint shaderID) {
    const size_t size = drawnObjects.size();
    if (size >= maxDrawnObjects)
        throw std::runtime_error("maximum number of drawn objects reached");
    drawnObjects.push_back(ObjectInstance(indexStoredObject, shaderID, defaultTransformation, defaultColor));
    return size;
}

// Sets the color for the drawn object of index indexDrawObject
void Scene::setDrawnObjectColor(GLuint indexDrawnObject, const Color &color) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].color = color;
}

// Sets the transformation matrix for the drawn object of index indexDrawObject
void Scene::setDrawnObjectModel(GLuint indexDrawnObject, float* model) {
    assert(indexDrawnObject<drawnObjects.size());
    assert(sizeof(model) / sizeof(float) == 16);
    mat4fCopy(drawnObjects[indexDrawnObject].transformation, model);
}

// Sets the if of the shader to use on the drawn object of index indexDrawObject
void Scene::setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID) {
    assert(indexDrawnObject<drawnObjects.size());
    drawnObjects[indexDrawnObject].shaderId = shaderID;
}

//Sets the ID of the texture to use on the drawn object of index indexDrawnObject
void Scene::setDrawnObjectTextureID(GLuint indexDrawnObject, GLuint textureUnit, GLuint textureID) {
	if (textureUnit == 0) drawnObjectsTexture0IDs[indexDrawnObject] = textureID;
	if (textureUnit == 1) drawnObjectsTexture1IDs[indexDrawnObject] = textureID;
}

// Sets the light
void Scene::setDefaultColor(const Color &color) {
    defaultColor = color;   
    for (size_t i = 0; i < drawnObjects.size(); ++i)
        drawnObjects[i].color = defaultColor;
}

// Sets default transformation matrix
void Scene::setDefaultModel(const float* _defaultModel) {
     assert(sizeof(_defaultModel) / sizeof(float) == 16);
    mat4fCopy(defaultTransformation, _defaultModel);
    for (size_t i = 0; i < drawnObjects.size(); ++i)
        drawnObjects[i].transformation = defaultTransformation;
}

// Sets default shader ID
void Scene::setDefaultShaderID(GLuint id) {
    defaultShaderID = id;
}

// Sets light data to use in shader
void Scene::setLight(GLfloat * position, GLfloat power) {
    for (GLuint iCoord = 0; iCoord < 4; iCoord++)
        lightPosition[iCoord] = position[iCoord];
    lightPower = power;
}

// Decides what will elements drawn after this call will look like
/*void Scene::setAppearance(const ObjectInstance &instance) {
    const size_t shaderId = instance.shaderId;

    // We use the specific values of model per object
    setMatricesInShader(shaderId, instance.transformation, pCamera->view(), pCamera->position(), pCamera->perspectiveProjection());
    glUniform4fv(glGetUniformLocation(shaderId, "color"), 1, instance.color);

    // Specifies which VBO were filled
    const Object * pObject = storedObjects[instance.objectId];
    assert(pObject);
    setFilledDataInShader(shaderId, pObject->hasPrimitives(), pObject->hasNormals(), pObject->hasUvs(), pObject->hasColors());
    
    // Sets the light in the current shader
    setLightInShader(shaderId, lightPosition, lightPower);
    
    //Selects our current texture for unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, drawnObjectsTexture0IDs[instance.objectId]);
    
    //Selects our current texture for unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, drawnObjectsTexture1IDs[instance.objectId]);
    
    setTextureUnitsInShader(shaderId);
    
    glCullFace(GL_BACK);
}*/

// Draw all Objects
void Scene::drawObjectsOfScene() {
	for (size_t i = 0; i < drawnObjects.size(); ++i) {
		const ObjectInstance &instance = drawnObjects[i];
		//setAppearance(instance);
		storedObjects[instance.objectId]->drawObject();
	}
}
