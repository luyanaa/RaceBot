#include "gl_tool/glModule.h"
#include <cstdio>
#include <cstring>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace glTool {
void Blueprint::bind(VertexArray &VAO) { VAO.bind(VBO, EBO); }
Substance::Substance() {
	scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	location = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	rotation_angle = 0.0f;
}
void Substance::setScaling(float x, float y, float z) {
	scaling = glm::vec3(x, y, z);
}
void Substance::setScaling(float x) { scaling = glm::vec3(x, x, x); }
void Substance::setLocation(float x, float y, float z) {
	location = glm::vec3(x, y, z);
	return;
}
void Substance::setRotation(float x, float y, float z, float angle) {
	rotation = glm::vec3(x, y, z);
	rotation_angle = angle;
	return;
}
void Module::draw(GLuint program, const GLchar *name) {
	glUseProgram(program);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, location);
	trans = glm::rotate(trans, glm::radians(rotation_angle), rotation);
	trans = glm::scale(trans, scaling);

	for (Substance *sub : subList)
		sub->draw(program, name, trans);
}
void Module::draw(GLuint program, const GLchar *name, glm::mat4 trans0) {
	glUseProgram(program);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, location);
	trans = glm::rotate(trans, glm::radians(rotation_angle), rotation);
	trans = glm::scale(trans, scaling);

	for (Substance *sub : subList)
		sub->draw(program, name, trans0 * trans);
}
void Module::addSubstance(Substance &sub) { subList.push_front(&sub); }
void Module::clear() { subList.clear(); }
void Module::setAttribute(unsigned int index, unsigned int size, AttrFillFunc func,
						  GLenum usage) {
	for (Substance *sub : subList)
		sub->setAttribute(index, size, func, usage);
}
} // namespace gl