#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include<iostream>

class Camera
{
private:
	bool key_event;
	GLfloat delta_X;
	GLfloat delta_Y;
	GLfloat speed_X;
	GLfloat speed_Y;

public:
	GLfloat camera_X;
	GLfloat camera_Y;

	GLfloat camera_x;
	GLfloat camera_y;
	GLfloat camera_z;

	Camera();
	Camera(GLfloat x, GLfloat y, GLfloat z);

	void MouseMove(int x, int y);
	void MouseButton(int button, int state, int x, int y);
	void keypress(unsigned char key, int x, int y);
	void setSpeed(GLfloat _speed_X, GLfloat _speed_Y);
	GLfloat getSpeedX();
	GLfloat getSpeedY();
	bool getKeyEnable();
};
#endif