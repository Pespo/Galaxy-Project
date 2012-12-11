#include "GalaxyApp.hpp"

#include <stein/Object.hpp>
#include <stein/Tools.hpp>
#include <stein/Builders.hpp>
#include <stein/math/StreamUtils.h>

#include <iostream>

#include <vector>

using namespace std;
using namespace stein;

GalaxyApp::GalaxyApp() :
    Application(800, 800) {

    const float size = .06;
    _scene.camera.setPerspectiveProjection(-size, size, -size, size, .1, 100);
    _scene.camera.setPosition(Vector3f(0, 0, 55));
    _scene.setDefaultShaderID(loadProgram("shaders/1.glsl"));

    Object &object = _scene.createObject(GL_TRIANGLES);
	MeshBuilder meshBuilder = MeshBuilder();
    buildSquare(object, 0.1, meshBuilder);

    _scene.addObjectToDraw(object.id);
    _scene.setDrawnObjectColor(object.id, Color(frand(), frand(), frand()));
    
}

void GalaxyApp::animate() {

}