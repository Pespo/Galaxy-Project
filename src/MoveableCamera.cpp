#include "MoveableCamera.hpp"
#include "GalaxyApp.hpp"
#include "GL/glfw.h"
#define M_PI  3.14159265f

using namespace stein;
using namespace std;

MoveableCamera::MoveableCamera() :
	Camera(), m_nextMove(), m_xMousePosition(), m_yMousePosition(), MOVE_STEP(0.1)
{}

MoveableCamera::~MoveableCamera()
{}

void MoveableCamera::cancelMovement() {
	m_nextMove = Vector3f(0., 0., 0.);
}
void MoveableCamera::setMouseMovement(int deltaX, int deltaY) {
	m_xMousePosition += 2. * (deltaX / (GLfloat)GalaxyApp::WIDTH);
	m_yMousePosition += -2. * (deltaY / (GLfloat)GalaxyApp::HEIGHT);
}

void MoveableCamera::setKeyMovement(Direction to) {
	size_t axis;
	if(to == FORWARD || to == BACKWARD)
		axis = FORWARD;
	else if (to == RIGHT || to == LEFT)
		axis = RIGHT;
	else
		axis = UP;
	m_nextMove[axis] += (to == axis) ? 1. : -1.;
}

void MoveableCamera::move() {
	Vector3f cameraNewPos;

	float moveOnX = -m_nextMove[0] * MOVE_STEP;
	float moveOnY = m_nextMove[1] * MOVE_STEP;
	float moveOnZ = m_nextMove[2] * MOVE_STEP;
	for(size_t iCoord=0; iCoord<3; ++iCoord)
	{
		cameraNewPos[iCoord]=position[iCoord]
			+xAxis[iCoord]*moveOnX
			+yAxis[iCoord]*moveOnY
			+zAxis[iCoord]*moveOnZ;
	}
	
	Matrix4f rotateAroundX = xRotation(-float(m_yMousePosition * M_PI/2.));
	Matrix4f rotateAroundY = yRotation(float(m_xMousePosition * M_PI));
	Matrix4f translate = translation(cameraNewPos);

	view.setIdentity();
	view =  view * rotateAroundX * rotateAroundY * translate;

	for(size_t iCoord=0; iCoord<3; ++iCoord)
	{
		//Updates the axis with values in view
		xAxis[iCoord]=view(iCoord, 0);
		yAxis[iCoord]=view(iCoord, 1);
		zAxis[iCoord]=view(iCoord, 2);
		//Updates the position of the camera c
		position[iCoord] = cameraNewPos[iCoord];
	}
	cancelMovement();
}


/*
@TODO : Should replace move() when corrected
void MoveableCamera::rotate() {
	//Motion motion information retrieval
	//Comment if mouse motion events are enabled
	SDL_PumpEvents();
	int mouseRelX, mouseRelY;
	#ifdef __APPLE__
		int mystery = 0;
		SDL_GetRelativeMouseState(mystery, &mouseRelX, &mouseRelY);
	#else
		SDL_GetRelativeMouseState(&mouseRelX, &mouseRelY);
	#endif

	m_xMousePosition += 2.0*mouseRelX/(GLfloat)GameApplication::WIDTH;
	m_yMousePosition += -2.0*mouseRelY/(GLfloat)GameApplication::HEIGHT;

	float angleLong = m_xMousePosition * M_PI;
	//float angleLat = m_yMousePosition * M_PI/2.;
	setRotation(yRotation(angleLong));

	// Method with rotates

	GLfloat xAxis[]={1.0,0.0,0.0};
	GLfloat yAxis[]={0.0,1.0,0.0};
	GLfloat rotateAroundX[16], rotateAroundY[16];
	
	setToRotate(rotateAroundX, -angleLat, xAxis);
	setToRotate(rotateAroundY, angleLong, yAxis);

	setToIdentity(this->scene->camera->view);
	multMatrixBtoMatrixA(this->scene->camera->view,rotateAroundX);
	multMatrixBtoMatrixA(this->scene->camera->view,rotateAroundY);
	
	for(GLuint iCoord=0; iCoord<3; iCoord++)
	{
		//Updates the axis with values in view
		this->scene->camera->x[iCoord]=this->scene->camera->view[iCoord*4+0];
		this->scene->camera->y[iCoord]=this->scene->camera->view[iCoord*4+1];
		this->scene->camera->z[iCoord]=this->scene->camera->view[iCoord*4+2];
		//Updates the position of the camera c
		this->scene->camera->c[iCoord]=cameraNewPos[iCoord];

	}
	
}

void MoveableCamera::translate() {
	Vector3f nextGlobalMove(m_nextMove);
	nextGlobalMove[FORWARD] *= -1; // Inverse the forward/backward axis
	//nextGlobalMove[RIGHT] *= -1; // Inverse the right/left axis
	//nextGlobalMove[UP] *= -1; // Inverse the up/down axis
	nextGlobalMove.normalize();
	// @TODO : Switch from local coordinates to global coordinates then :
	
	setPosition(getPosition() + nextGlobalMove * MOVE_STEP);
}
*/
