#include "Camera.h"

Camera::Camera() {
	camera_x = 0.0f;
	camera_y = 0.0f;
	camera_z = -100.0f;
	camera_X = 0.0f;
	camera_Y = 0.0f;
	delta_X = 0.0f;
	delta_Y = 0.0f;
	speed_X = 0.5f;
	speed_Y = 0.5f;
	key_event = FALSE;
}

Camera::Camera(GLfloat x, GLfloat y, GLfloat z) {
	camera_x = x;
	camera_y = y;
	camera_z = z;
	camera_X = 0.0f;
	camera_Y = 0.0f;
	delta_X = 0.0f;
	delta_Y = 0.0f;
	speed_X = 0.5f;
	speed_Y = 0.5f;
	key_event = FALSE;
}

bool Camera::getKeyEnable() {
	return key_event;
}

void Camera::MouseMove(int x, int y){
	camera_X += (x - delta_X) * speed_X;
	camera_Y += (y - delta_Y) * speed_Y;
	delta_X = x;
	delta_Y = y;
}

void Camera::MouseButton(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			delta_X = x;
			delta_Y = y;
		}
	}
}

void Camera::keypress(unsigned char key, int x, int y){
	switch (key)
	{
	case 'x':
		key_event = !key_event;
		break;
	case 'w':
		camera_z++;
		break;
	case 's':
		camera_z--;
		break;
	case 'a':
		camera_x--;
		break;
	case 'd':
		camera_x++;
		break;
	case 'r':
		camera_x = 0;
		camera_z = 0;
	default:
		break;
	}
}

void Camera::setSpeed(GLfloat _speed_X, GLfloat _speed_Y) {
	speed_X = _speed_X;
	speed_Y = _speed_Y;
}

GLfloat Camera::getSpeedX() {
	return speed_X;
}

GLfloat Camera::getSpeedY() {
	return speed_Y;
}