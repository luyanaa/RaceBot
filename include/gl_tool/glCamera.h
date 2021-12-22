#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace glTool {

class Camera {
public:
	Camera();
	Camera(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up,
		   const glm::mat4 &perspective);
	void setEye(const float &x, const float &y, const float &z);
	void setCenter(const float &x, const float &y, const float &z);
	void setUp(const float &x, const float &y, const float &z);
	void setperspective(const float &fovy, const float &aspect,
						const float &near, const float &far);
	// 不推荐，建议使用getMat()
	void use(GLuint program, const GLchar *name);
	glm::mat4 getMat();
	glm::vec3 eye;
	
	bool lazy;
	glm::mat4 mat, perspective;
	glm::vec3 center, up;

};
} // namespace glTool
#endif
