#include "Tools.hpp"
#include "stb_image/stb_image.h"
#include "GL/glfw.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

namespace stein {

// Sets the provided matrix to a translate matrix on vector t
Matrix4f translation(const Vector3f&t) {
    return Matrix4f( //
        1, 0, 0, 0, //
        0, 1, 0, 0, //
        0, 0, 1, 0, //
        t.x, t.y, t.z, 1);
}

// Sets the provided matrix to a scale matrix by coeficients in s
Matrix4f scale(const Vector3f&s) {
    return Matrix4f( //
        s.x, 0, 0, 0, //
        0, s.y, 0, 0, //
        0, 0, s.z, 0, //
        0, 0, 0, 1);
}

Matrix4f xRotation(const float angle) {
    const float c = cos(angle);
    const float s = sin(angle);
    return Matrix4f( //
        1, 0, 0, 0, //
        0, c, s, 0, //
        0, -s, c, 0, //
        0, 0, 0, 1);
}
Matrix4f yRotation(const float angle) {
    const float c = cos(angle);
    const float s = sin(angle);
    return Matrix4f( //
        c, 0, -s, 0, //
        0, 1, 0, 0, //
        s, 0, c, 0, //
        0, 0, 0, 1);
}
Matrix4f zRotation(const float angle) {
    const float c = cos(angle);
    const float s = sin(angle);
    return Matrix4f( //
        c, s, 0, 0, //
        -s, c, 0, 0, //
        0, 0, 1, 0, //
        0, 0, 0, 1);
}

Matrix4f rotation(const float angle, const Vector3f &axis) {
    const float c = cos(angle);
    const float s = sin(angle);
    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;
    return Matrix4f( //
        (1 - c) * (x * x - 1) + 1, (1 - c) * x * y + (z * s), (1 - c) * x * z - (y * s), 0, //
        (1 - c) * x * y - (z * s), (1 - c) * (y * y - 1) + 1, (1 - c) * y * z + (x * s), 0, //
        (1 - c) * x * z + (y * s), (1 - c) * y * z - (x * s), (1 - c) * (z * z - 1) + 1, 0, //
        0, 0, 0, 1);
}

// Setup an orthographic projection in matA
void ortho(float left, float right, float bottom, float top, float nearVal, float farVal, float * matA) {
    float tx = -(right + left) / ( right - left);
    float ty = -(top + bottom) / ( top - bottom);
    float tz = -(farVal + nearVal) / ( farVal - nearVal);
    matA[0] = 2.0f/(right-left); matA[1] = 0.0f; matA[2] = 0.0f; matA[3] = 0.0f;
    matA[4] = 0.0f; matA[5] = 2.0f/(top-bottom); matA[6] = 0.0f; matA[7] = 0.0f;
    matA[8] = 0.0f; matA[9] = 0.0f; matA[10] = -2.0f/(farVal-nearVal); matA[11] = 0.0f;
    matA[12] = tx; matA[13] = ty; matA[14] = tz; matA[15] = 1.0f;
}

// Builds a perspective projection matrix and stores it in mat
// l=left, r=right, b=bottom, t=top, n=near, f=far in the frustum
void setPerspective(GLfloat * mat, GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f) {
    GLfloat P[] = { (2 * n) / (r - l), 0, 0, 0, 0, (2 * n) / (t - b), 0, 0, (r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1, 0, 0, 0, 0 };
    for (int iMatrixCoord = 0; iMatrixCoord < 16; iMatrixCoord++)
        mat[iMatrixCoord] = P[iMatrixCoord];
}

// Prints the stack of glErrors, or nothing if no error occured
void printGlErrors() {
    GLenum error = glGetError();
    
    // The glGetError function returns one error at a time, and then unstacks it.
    // Here we call it until all the errors are shown.
    while (error != GL_NO_ERROR) {
        cerr << "!! GL Error : ";
        if (error == GL_INVALID_ENUM)
            cerr << "GL_INVALID_ENUM" << endl;
        if (error == GL_INVALID_VALUE)
            cerr << "GL_INVALID_VALUE" << endl;
        if (error == GL_INVALID_OPERATION)
            cerr << "GL_INVALID_OPERATION" << endl;
        if (error == GL_OUT_OF_MEMORY)
            cerr << "GL_OUT_OF_MEMORY" << endl;

        error = glGetError();

        cerr << endl;
        //exit(1);
    }
}

// Loads a simple texture
GLuint loadTexture(const char* fileName, const int &comp) {
    int w;
    int h;
    int c;
    unsigned char *data;
    
   /* try {
		// Loads the image from a ppm file to an unsigned char array
		data = stbi_load(fileName, &w, &h, &c, comp);
	 }
	 catch (...) {
		 return 0;
	}*/

	//Selects our current unit texture
	glActiveTexture(GL_TEXTURE0);
	

        
    // Allocates a texture id
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    // Selects our current texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glfwLoadTexture2D( fileName , GLFW_BUILD_MIPMAPS_BIT);
    // How to handle not normalised uvs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // How to handle interpolation from texels to fragments
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Specifies which image will be used for this texture objet
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  
    return textureID;
}

// Writes content of a text file [fileName] in a returned string
string* loadFile(const string &fileName) {
    string* result = new string();
    ifstream file(fileName.c_str());
    if (!file) {
        cerr << "Cannot open file " << fileName << endl;
        throw exception();
    }
    string line;
    while (getline(file, line)) {
        *result += line;
        *result += '\n';
    }
    file.close();
    return result;
}

// Prints info about potential problems which occured during compilation
void printShaderLog(GLuint shaderId) {
    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        char *log = new char[logLength];
        glGetShaderInfoLog(shaderId, logLength, &logLength, log);
        cout << string(log);
        delete[] log;
    }
}

GLuint loadProgram(const string &file) {
    vector<string> files;
    files.push_back(file);
    return loadProgram(files);
}

// Loads files [files] in string table, builds program object, compile shaders in shaders objects, links and return the id program object (with the executable code)
GLuint loadProgram(const vector<string> &files) {
    GLuint programId = glCreateProgram(); /// Creates a program object which id is returned

    glBindAttribLocation(programId, 0, "vertexPosition");
    glBindAttribLocation(programId, 1, "vertexNormal");
    glBindAttribLocation(programId, 2, "vertexUv");
    glBindAttribLocation(programId, 3, "vertexColor");

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER); /// Creates a vertex shader object which id is returned
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); /// Creates a fragment shader object which id is returned
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    GLint n = files.size();
    string **strs = new string*[n];
    const char** lines = new const char*[n + 1];
    
    bool geo = false;
    for (int i = 0; i < n; ++i) /// For every file :
        {
        string* s = loadFile(files[i]); /// get string of file
        strs[i] = s; /// Store it [string] in strs[i]
        lines[i + 1] = s->c_str(); /// Store it [char] in lines[i+1]

        /// If _GEOMETRY_ is in file, geometrey shader : geo=true
        if (strstr(lines[i + 1], "_GEOMETRY_") != NULL) /// strstr(a, b)-> finds firt occurence of b in a
            geo = true;
    }

    lines[0] = "#define _VERTEX_\n";
    glShaderSource(vertexShaderId, n + 1, lines, NULL); /// Loads shader source chars in shader object
    glCompileShader(vertexShaderId); /// Compile the loaded shader source code
    printShaderLog(vertexShaderId); /// Prints compilation potential problems

    if (geo) {
        unsigned int geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT); /// Creates a geometry shader object which id is returned
        glAttachShader(programId, geometryShaderId);
        lines[0] = "#define _GEOMETRY_\n"; // Adds this text before the source text
        glShaderSource(geometryShaderId, n + 1, lines, NULL);
        glCompileShader(geometryShaderId);
        printShaderLog(geometryShaderId);
        glProgramParameteriEXT(programId, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES); /// Specifies type of primitives accessible in geometry shader
    }

    lines[0] = "#define _FRAGMENT_\n";
    glShaderSource(fragmentShaderId, n + 1, lines, NULL);
    glCompileShader(fragmentShaderId);
    printShaderLog(fragmentShaderId);

    glLinkProgram(programId); /// Links the program object to build the executable code

    for (int i = 0; i < n; ++i) {
        delete strs[i];
    }
    delete[] strs;
    delete[] lines;

    return programId; /// Returns the id of the program object
}

// Passing the matrices to the shader
void setMatricesInShader(GLuint shaderID, const Matrix4f &model, const Matrix4f &view, const Vector3f &cameraPosition, const Matrix4f &projection) {
    GLenum toTranspose = GL_FALSE;
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, toTranspose, model);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, toTranspose, (const float*) view);
    glUniform3fv(glGetUniformLocation(shaderID, "cameraPosition"), 1, (const float*) cameraPosition);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, toTranspose, (const float*) projection);
}

// Passing the light to the shader
void setLightInShader(GLuint shaderID, Light* light) {
    // Passing the light to the shader
    glUniform4fv(glGetUniformLocation(shaderID, "light.position"), 1, light->position);
    std::cout << "l : " << light->position[0] << std::endl;
    glUniform1f(glGetUniformLocation(shaderID, "light.power"), light->power);
}

// Passing the material to the shader
void setMaterialInShader(GLuint shaderID, GLuint mat) {
    glUniform1ui(glGetUniformLocation(shaderID, "material"), mat);
}

// Change color in the material in the shader (diffuse component)
void changeMaterialColorInShader(GLuint shaderID, GLfloat * color) {
    glUniform4fv(glGetUniformLocation(shaderID, "material.diffuse"), 1, color);
}

// Set filledData in the shader (flag saying with data is provided as attribute)
void setFilledDataInShader(GLuint shaderID, GLboolean positions, GLboolean normals, GLboolean uvs, GLboolean colors) {
    glUniform4i(glGetUniformLocation(shaderID, "filledData"), positions, normals, uvs, colors);
}

// Sets the units of the textures to use for diffuse and specular as a uniform samplers in shader
void setTextureUnitsInShader(GLuint shaderId) {
    glUniform1i(glGetUniformLocation(shaderId, "textureUnitDiffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderId, "textureUnitSpecular"), 1);
}

//______________________________________________________________________________
// PPM file reading fuction

// Loads an image encoded in ppm format (P6)
unsigned char * loadPPM(const char* filename, GLuint &width, GLuint &height) {
    unsigned char * data;
    // Reads header
    ifstream file(filename);
    string line;

    // Formats
    getline(file, line);

    // Skips comments
    getline(file, line);
    while (line[0] == '#')
        getline(file, line);

    // Reads dimensions
    istringstream ist(line);
    ist >> width >> height;

    // Reads the data
    getline(file, line);

    /* May be necessary on windows	   
     // 	unsigned int dataStart = file.tellg();
     // 	file.close();
     // 	file.open(filename.c_str(),ios::in | ios::binary);
     // 	file.seekg(dataStart);
     */

    data = new unsigned char[width * height * 3];
    file.read((char*) data, width * height * sizeof(unsigned char) * 3);

    // Closes the file
    file.close();

    return data;
}

// needs -lrt (real-time lib)
// 1970-01-01 epoch UTC time, 1 microsecond resolution (divide by 1M to get time_t)
//uint64_t getTime() {
//    timespec ts;
    
    // apple version was not tested and windows version is to be done
#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
#else
//    clock_gettime(CLOCK_REALTIME, &ts);
#endif

   // return ((uint64_t) ts.tv_sec * 1000000LL) + ((uint64_t) ts.tv_nsec / 1000LL);
//}

} // namespace stein
