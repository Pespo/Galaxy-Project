#pragma once

#include "GLHeaders.hpp"

struct Material {
    GLfloat* ambient; 
    GLfloat* diffuse; 
    GLfloat* specular; 
    GLfloat ka, kd, ks, shininess;

    Material() : ambient(NULL), diffuse(NULL), specular(NULL) {}
    Material(GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat shininess) :
    ambient(ambient), diffuse(diffuse), specular(specular), ka(ka), kd(kd), ks(ks), shininess(shininess) {}
    ~Material() {
        delete [] ambient;
        delete [] diffuse;
        delete [] specular;
    }

    void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
        ambient[0] = r;
        ambient[1] = g;
        ambient[2] = b;
        ambient[3] = a;
    }

    void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
        diffuse[0] = r;
        diffuse[1] = g;
        diffuse[2] = b;
        diffuse[3] = a;
    }

    void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
        specular[0] = r;
        specular[1] = g;
        specular[2] = b;
        specular[3] = a;
    }
};