#include <string.h>
#include <iostream>
#include "common/GLCommon.hpp"
#include "GL/glfw.h"
#include "common/Application.hpp"
#include "common/Camera.hpp"

#ifndef DEBUG_PRINT
#define DEBUG_PRINT 1
#endif

#if DEBUG_PRINT == 0
#define debug_print(FORMAT, ...) ((void)0)
#else
#ifdef _MSC_VER
#define debug_print(FORMAT, ...) \
    fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
        __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#else
#define debug_print(FORMAT, ...) \
    fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
        __func__, __FILE__, __LINE__, __VA_ARGS__)
#endif 
#endif 

int main( int argc, char **argv )
{
<<<<<<< HEAD:src/main.cpp
    // Basic settings initialization
    const size_t width = 1024, height = 768 ;
    char* title = "Galaxy Project - Guillaume Albespy & Guillaume Bréda";

    try {
        // Initialise GLFW
        Application galaxy(width, height, title);

        // @TODO: Move this to Galaxy's project's specific Application
            // Init viewer structures
            Camera camera;

            /************* Initialisation des textures *****************/
            // Load images and upload textures
            GLuint textures[3];
            glGenTextures(3, textures);
            textures[0] = galaxy.loadTexture("textures/spnza_bricks_a_diff.tga", 3);
            textures[1] = galaxy.loadTexture("textures/spnza_bricks_a_spec.tga", 1);

            /************* Initialisation des shaders*****************/   
            // Load gbuffer shader
            ShaderGLSL gbuffer_shader;
            galaxy.loadShader(gbuffer_shader, "galaxy_project/shader/3_gbuffer.glsl");
            // Compute locations for gbuffer_shader
            GLuint gbuffer_projectionLocation = glGetUniformLocation(gbuffer_shader.program, "Projection");
            GLuint gbuffer_viewLocation = glGetUniformLocation(gbuffer_shader.program, "View");
            GLuint gbuffer_objectLocation = glGetUniformLocation(gbuffer_shader.program, "Object");
            GLuint gbuffer_timeLocation = glGetUniformLocation(gbuffer_shader.program, "Time");
            GLuint gbuffer_diffuseLocation = glGetUniformLocation(gbuffer_shader.program, "Diffuse");
            GLuint gbuffer_specLocation = glGetUniformLocation(gbuffer_shader.program, "Spec");

            // Load Blit shader
            ShaderGLSL blit_shader;
            galaxy.loadShader(blit_shader, "galaxy_project/shader/3_blit.glsl");
            // Compute locations for blit_shader
            GLuint blit_projectionLocation = glGetUniformLocation(blit_shader.program, "Projection");
            GLuint blit_tex1Location = glGetUniformLocation(blit_shader.program, "Texture1");

            // Load light accumulation shader
            ShaderGLSL laccum_shader;
            galaxy.loadShader(laccum_shader, "galaxy_project/shader/3_laccum_spot.glsl");
            float shadowBias = 0.001f;
            // Compute locations for light accumulation shader
            GLuint laccum_projectionLocation = glGetUniformLocation(laccum_shader.program, "Projection");
            GLuint laccum_materialLocation = glGetUniformLocation(laccum_shader.program, "Material");
            GLuint laccum_normalLocation = glGetUniformLocation(laccum_shader.program, "Normal");
            GLuint laccum_depthLocation = glGetUniformLocation(laccum_shader.program, "Depth");
            GLuint laccum_shadowLocation = glGetUniformLocation(laccum_shader.program, "Shadow");
            GLuint laccum_inverseViewProjectionLocation = glGetUniformLocation(laccum_shader.program, "InverseViewProjection");
            GLuint laccum_cameraPositionLocation = glGetUniformLocation(laccum_shader.program, "CameraPosition");
            GLuint laccum_lightPositionLocation = glGetUniformLocation(laccum_shader.program, "LightPosition");
            GLuint laccum_lightDirectionLocation = glGetUniformLocation(laccum_shader.program, "LightDirection");
            GLuint laccum_lightColorLocation = glGetUniformLocation(laccum_shader.program, "LightColor");
            GLuint laccum_lightIntensityLocation = glGetUniformLocation(laccum_shader.program, "LightIntensity");
            GLuint laccum_projectionLightBiasLocation = glGetUniformLocation(laccum_shader.program, "ProjectionLightBias");
            GLuint laccum_shadowBiasLocation = glGetUniformLocation(laccum_shader.program, "ShadowBias");

            // Load shadow generation shader
            ShaderGLSL shadowgen_shader;
            galaxy.loadShader(shadowgen_shader, "galaxy_project/shader/3_shadowgen.glsl");
            // Compute locations for shadow generation shader
            GLuint shadowgen_projectionLocation = glGetUniformLocation(shadowgen_shader.program, "Projection");
            GLuint shadowgen_viewLocation = glGetUniformLocation(shadowgen_shader.program, "View");
            GLuint shadowgen_objectLocation = glGetUniformLocation(shadowgen_shader.program, "Object");
            GLuint shadowgen_timeLocation = glGetUniformLocation(shadowgen_shader.program, "Time");
        // @ENDTODO //

        galaxy.loop();
        
        return EXIT_SUCCESS;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Something very bad happened..." << std::endl;
    }
    return EXIT_FAILURE;

    exit( EXIT_SUCCESS );
}

