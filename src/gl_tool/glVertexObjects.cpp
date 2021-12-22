#include "gl_tool/glVertexObjects.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace glTool {
// VBO
VertexBuffer::VertexBuffer() { glGenBuffers(1, &this->ID); };
VertexBuffer::VertexBuffer(GLsizeiptr size, const void *data, GLenum usage) {
	glGenBuffers(1, &this->ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
};
VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &this->ID); };
void VertexBuffer::enable() {
	glBindBuffer(GL_ARRAY_BUFFER, this->ID);
	return;
};
void VertexBuffer::data(GLsizeiptr size, const void *data, GLenum usage) {
	glBindBuffer(GL_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
};
// EBO

ElementBuffer::ElementBuffer() { glGenBuffers(1, &this->ID); };
ElementBuffer::ElementBuffer(GLsizeiptr size, const void *data, GLenum usage) {
	this->size = size;
	glGenBuffers(1, &this->ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
};
ElementBuffer::~ElementBuffer() { glDeleteBuffers(1, &this->ID); };
void ElementBuffer::enable() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
	return;
};
void ElementBuffer::data(GLsizeiptr size, const void *data, GLenum usage) {
	this->size = size;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
};

// VAO
VertexArray::VertexArray() {
	glGenVertexArrays(1, &this->ID);
	VBO = nullptr;
	EBO = nullptr;
}
void VertexArray::bind(VertexBuffer &VBO, ElementBuffer &EBO) {
	glBindVertexArray(this->ID);
	this->VBO = &VBO;
	this->EBO = &EBO;
	VBO.enable();
	EBO.enable();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);
}
VertexArray ::~VertexArray() { glDeleteVertexArrays(1, &this->ID); };
void VertexArray::enable() {
	glBindVertexArray(this->ID);
	if (VBO != nullptr && EBO != nullptr) {
		VBO->enable();
		EBO->enable();
	}
};
void VertexArray::draw(GLenum mode) {
	if (VBO != nullptr && EBO != nullptr) {
		glBindVertexArray(this->ID);
		VBO->enable();
		EBO->enable();
		glDrawElements(mode, EBO->size / sizeof(unsigned int), GL_UNSIGNED_INT,
					   0);
	}
	return;
};
void VertexArray::EnableAttribute(GLuint index, GLint size, GLenum type,
								  GLsizei stride, const void *offset) {
	glBindVertexArray(this->ID);
	glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(index);
	return;
};
} // namespace glTool