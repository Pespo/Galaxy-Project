#pragma once

//#include "GameApplication.hpp"
#include <stein/Camera.hpp>
#include <stein/math/Vector3f.hpp>
#include <cstdlib>
#include <iostream>
#include <stein/Tools.hpp>

// forward declaration
class GalaxyApplication;

enum Direction {
	RIGHT = 0, UP = 1, FORWARD = 2, LEFT = 3, DOWN = 4, BACKWARD = 5, NOWHERE = 6
};

class MoveableCamera : public stein::Camera {
protected :
	stein::Vector3f m_nextMove;
	float m_xMousePosition;
	float m_yMousePosition;
	 
public :
	float MOVE_STEP;
	float MOUSE_STEP;

	MoveableCamera();
	virtual ~MoveableCamera();
	void setMouseMovement(int deltaX, int deltaY);
	void setKeyMovement(Direction to);
	void cancelMovement();
	void move();
	/*
	virtual void translate();
	virtual void rotate();
	*/
};
