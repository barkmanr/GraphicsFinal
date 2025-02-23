#include "Camera.h"

Camera::Camera(Resolution _resolution) : Camera(_resolution, glm::vec3(4, 3, 3)) { }

Camera::Camera(Resolution _resolution, glm::vec3 _camLocation)
{
	curentRes = _resolution;
	CreateProjection(curentRes, 0.1f, 1000.0f);
	position = _camLocation;
	lookAt = - position;
	angle = 0.1f;
	CreateView(position, lookAt, glm::vec3(0, 1, 0));
}

void Camera::CreateView(glm::vec3 _position, glm::vec3 _target, glm::vec3 _up)
{
	view = glm::lookAt(_position, _target, _up);
}

void Camera::CreateProjection(Resolution _res, float _nearP, float _farP)
{
	projection = glm::perspective(glm::radians(_res.fov), _res.width / _res.height, _nearP, _farP);
}

void Camera::ChangeFov()
{
	curentRes.fov += 15;
	if (curentRes.fov > 180)
	{
		curentRes.fov = 15;
	}

	CreateProjection(curentRes, 0.1f, 1000.0f);
}

void Camera::ChangePosition(const glm::vec3 _location)
{
	position = _location;
	CreateView(position, lookAt, glm::vec3(0, 1, 0));
}

void Camera::ChangeLookAt(const glm::vec3 _location)
{
	lookAt = _location;
	CreateView(position, lookAt, glm::vec3(0, 1, 0));
}

void Camera::Rotate(float dtime)
{
	float rad = dtime/4;
	glm::mat3 rotateY = { {cos(rad),0,sin(rad)}, {0,1,0}, {-sin(rad), 0, cos(rad)} };
	
	glm::vec3 v = lookAt - position;
	v = rotateY * v;
	v = v + position;
	lookAt = v;
	CreateView(position, lookAt, glm::vec3(0, 1, 0));
}

void Camera::RotatePos()
{
	float rad = glm::radians(angle);
	glm::mat3 rotateY = { {cos(rad),0,sin(rad)}, {0,1,0}, {-sin(rad), 0, cos(rad)} };
	position = rotateY * position;
	position = glm::normalize(position);
	position *= 5; //could mull

	CreateView(position, lookAt, glm::vec3(0, 1, 0));
}