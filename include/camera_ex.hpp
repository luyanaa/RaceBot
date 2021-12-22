#ifndef CAMERA_EX_H
#define CAMERA_EX_H

#include "gl_tool/glCamera.h"
#include "glm/gtc/matrix_transform.hpp"
class CameraEX {
public:
	glTool::Camera cam;
	bool lazy;
	glm::vec3 loc;
	glm::vec3 dir;
	glm::vec3 up;
	float fov, asp, near, far;

	CameraEX() {
		loc = glm::vec3(1, 0, 0);
		dir = glm::vec3(-1, 0, 0);
		up = glm::vec3(0, 1, 0);
		fov = glm::radians(45.0f);
		asp = 1.0f;
		near = 0.1f;
		far = 100.0f;
		lazy = 1;
	};
	CameraEX(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up,
			 float fov, float asp, float near, float far) {
		loc = eye;
		dir = glm::normalize(center - eye);
		this->up = up;
		this->fov = fov;
		this->asp = asp;
		this->near = near;
		this->far = far;
		lazy = 1;
	};
	void setperspective(const float &aspect, const float &near,
						const float &far) {
		this->asp = asp;
		this->near = near;
		this->far = far;
		lazy = 1;
	}

	void forword(float d) {
		loc += dir * d;
		lazy = 1;
	}
	void right(float d) {
		loc += glm::normalize(glm::cross(dir, up)) * d;
		lazy = 1;
	}
	void high(float d) {
		loc += glm::cross(glm::normalize(glm::cross(dir, up)), dir) * d;
		lazy = 1;
	}
	void viewHigh(float d) {
		dir = glm::mat3(glm::rotate(glm::mat4(1), glm::radians(d),
									glm::cross(dir, up))) *
			  dir;
		up = glm::cross(glm::normalize(glm::cross(dir, up)), dir);
		lazy = 1;
	};
	void viewRight(float d) {
		dir = glm::mat3(glm::rotate(
				  glm::mat4(1), -glm::radians(d),
				  glm::cross(glm::normalize(glm::cross(dir, up)), dir))) *
			  dir;
		up = glm::cross(glm::normalize(glm::cross(dir, up)), dir);
		lazy = 1;
	}
	void viewForward(float d) {
		fov += d;
		if (fov < 1) {
			fov = 1;
		}
		if (fov > 180) {
			fov = 179;
		}
		lazy = 1;
	}
	void viewClock(float d) {
		up = glm::cross(glm::normalize(glm::cross(dir, up)), dir);
		up = glm::mat3(glm::rotate(glm::mat4(1), glm::radians(d), dir)) * up;
		lazy = 1;
	}
	
	glm::mat4 getMat() {
		if (lazy) {
			cam.setEye(loc.x, loc.y, loc.z);
			cam.setCenter(loc.x + dir.x, loc.y + dir.y, loc.z + dir.z);
			cam.setUp(up.x, up.y, up.z);
			cam.setperspective(fov, asp, near, far);
			lazy = 0;
		}
		return cam.getMat();
	}
};
#endif