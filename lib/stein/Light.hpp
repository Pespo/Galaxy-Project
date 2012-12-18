#pragma once

#include "GLHeaders.hpp"

struct Light {
    GLfloat* position; // Position of the light used in shader
    GLfloat power; // Power of the light used in shader

    Light() : position(NULL) {}
    Light(GLfloat* pos, GLfloat pow) : position(pos), power(pow) {}
    ~Light() {
        delete [] position;
    }

    void setPosition(GLfloat x, GLfloat y, GLfloat z) {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        position[3] = 1.0;
    }

};