#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__

// To include GL
#include "GLHeaders.hpp"

#include "math/Matrix4f.hpp"
#include "math/Vector3f.hpp"

#include <stein/Scene.hpp>

// C++
#include <string>
#include <vector>

namespace stein {

Matrix4f translation(const Vector3f&t);
Matrix4f scale(const Vector3f&s);
Matrix4f xRotation(const float angle);
Matrix4f yRotation(const float angle);
Matrix4f zRotation(const float angle);
Matrix4f rotation(const float angle, const Vector3f &axis);

Matrix4f orthoP(float left, float right, float bottom, float top, float nearVal, float farVal);

void ortho(float left, float right, float bottom, float top, float nearVal, float farVal, float * matA);

Matrix4f lookAt(Vector3f &vecEye, Vector3f &vecCenter, Vector3f &vecUp);
Matrix4f perspective(float fovy, float aspect, float nearVal, float farVal) ;

void printGlErrors();

GLuint loadTexture(const char* fileName);
GLuint loadTextureCube(/*const char* fileName[6]*/);
GLuint loadProgram(const std::vector<std::string> & files);
GLuint loadProgram(const std::string& file);
void printShaderLog(GLuint shaderId);

std::string * loadFile(const std::string & fileName);
unsigned char * loadPPM(const char* filename, GLuint &width, GLuint &height);

void setMatricesInShader(GLuint shaderId, const Matrix4f &model, const Matrix4f &view, const Vector3f &cameraPosition, const Matrix4f &projection);
void setLightInShader(GLuint shaderID, Light* light);
void setMaterialInShader(GLuint shaderID, GLuint material);
void setFilledDataInShader(GLuint shaderID, GLboolean positions, GLboolean normals, GLboolean uvs, GLboolean colors);
void setTextureUnitsInShader(GLuint shaderID);

void changeMaterialColorInShader(GLuint shaderID, GLfloat * color);

//uint64_t getTime();

static inline float frand() {
    return float(rand()) / RAND_MAX;
}

} // namespace stein

#endif //  __TOOLS_HPP__
