#include <glad/glad.h>

#include "gl_tool/glCamera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace glTool {

Camera::Camera() {
	lazy = 0;
	this->eye = glm::vec3(1, 0, 0);
	this->center = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	this->perspective =
		glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	this->mat =
		this->perspective * glm::lookAt(this->eye, this->center, this->up);
}

Camera::Camera(const glm::vec3 &eye, const glm::vec3 &center,
			   const glm::vec3 &up, const glm::mat4 &perspective) {
	lazy = 0;
	this->eye = eye;
	this->center = center;
	this->up = up;
	this->perspective = perspective;

	this->mat =
		this->perspective * glm::lookAt(this->eye, this->center, this->up);
}
void Camera::setEye(const float &x, const float &y, const float &z) {
	lazy = 1;
	this->eye = glm::vec3(x, y, z);
	return;
}
void Camera::setCenter(const float &x, const float &y, const float &z) {
	lazy = 1;
	this->center = glm::vec3(x, y, z);
	this->mat =
		this->perspective * glm::lookAt(this->eye, this->center, this->up);
	return;
}
void Camera::setUp(const float &x, const float &y, const float &z) {
	lazy = 1;
	this->up = glm::vec3(x, y, z);
	this->mat =
		this->perspective * glm::lookAt(this->eye, this->center, this->up);
	return;
}
void Camera::setperspective(const float &fovy, const float &aspect,
							const float &near, const float &far) {
	lazy = 1;
	this->perspective = glm::perspective(glm::radians(fovy), aspect, near, far);

	return;
}
void Camera::use(GLuint program, const GLchar *name) {
	if (lazy) {
		this->mat =
			this->perspective * glm::lookAt(this->eye, this->center, this->up);
		lazy = 0;
	}
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE,
					   glm::value_ptr(this->mat));
	return;
}
glm::mat4 Camera::getMat() {
	if (lazy) {
		this->mat =
			this->perspective * glm::lookAt(this->eye, this->center, this->up);
		lazy = 0;
	}
	return this->mat;
}
} // namespace glTool