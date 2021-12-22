#ifndef VERTEXOBJECTS_H
#define VERTEXOBJECTS_H

#include <glad/glad.h>
namespace glTool {

class VertexBuffer {
public:
	unsigned int ID;
	VertexBuffer();
	VertexBuffer(GLsizeiptr size, const void *data, GLenum usage);
	~VertexBuffer();
	void enable();
	void data(GLsizeiptr size, const void *data, GLenum usage);
};
class ElementBuffer {
public:
	unsigned int ID;
	unsigned int size;
	ElementBuffer();
	ElementBuffer(GLsizeiptr EBOSize, const void *EBOData, GLenum EBOUsage);
	void data(GLsizeiptr EBOSize, const void *EBOData, GLenum EBOUsage);
	~ElementBuffer();
	void enable();
};
class VertexArray {
public:
	unsigned int ID;
	VertexBuffer *VBO;
	ElementBuffer *EBO;
	VertexArray();
	~VertexArray();
	void enable();
	void draw(GLenum mode);
	void bind(VertexBuffer &VBO,ElementBuffer &EBO);
	void EnableAttribute(GLuint index, GLint size, GLenum type, GLsizei stride,
						 const void *offset);
};
} // namespace gl

#endif