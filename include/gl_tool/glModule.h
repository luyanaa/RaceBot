#ifndef MODULE_H
#define MODULE_H

#include "gl_tool/glVertexObjects.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>

#include <cstdio>
namespace glTool
{
	// Blueprint 只存储模型顶点，不处理缩放旋转相关
	class Blueprint
	{
	public:
		VertexBuffer VBO;
		ElementBuffer EBO;
		virtual const float *getVertixBufferData() const = 0;
		virtual const unsigned int *getVertixElementData() const = 0;
		virtual int getVertixBufferSize() const = 0;
		virtual int getVertixElementSize() const = 0;
		void bind(VertexArray &VAO);
		GLenum usage;
	};

	// Substance 为一个可以输出的实体，包含处理缩放旋转相关
	typedef void (*AttrFillFunc)(float x, float y, float z, float *out);
	class Substance
	{
	public:
		Substance();
		void setScaling(float x);
		void setScaling(float x, float y, float z);
		void setLocation(float x, float y, float z);
		void setRotation(float x, float y, float z, float angle);

		virtual void draw(GLuint program, const GLchar *name) = 0;
		virtual void draw(GLuint program, const GLchar *name, glm::mat4 trans) = 0;
		virtual void setAttribute(unsigned int index, unsigned int size,
								  AttrFillFunc func, GLenum usage) = 0;

	protected:
		glm::vec3 scaling;
		glm::vec3 rotation;
		float rotation_angle;
		glm::vec3 location;
	};
	// Part 包含一个蓝图
	// exAttrCnt 为需要另外给予 VAO 参数的个数
	template <int exAttrCnt>
	class Part : public Substance
	{
	public:
		Blueprint *bp;
		Part(Blueprint &sub)
		{
			bp = &sub;
			bp->bind(VAO);
			if (exAttrCnt)
				VBO = new VertexBuffer[exAttrCnt]();
		}
		Part()
		{
			if (exAttrCnt)
				VBO = new VertexBuffer[exAttrCnt]();
		}
		void setBlueprint(Blueprint &sub)
		{
			bp = &sub;
			bp->bind(VAO);
			if (exAttrCnt)
				VBO = new VertexBuffer[exAttrCnt]();
		}

		~Part()
		{
			if (exAttrCnt)
			delete[] VBO;
		}
		void draw(GLuint program, const GLchar *name)
		{
			glUseProgram(program);
			VAO.enable();
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, location);
			trans = glm::rotate(trans, glm::radians(rotation_angle), rotation);
			trans = glm::scale(trans, scaling);
			// ここだよ
			glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE,
							   glm::value_ptr(trans));
			VAO.draw(bp->usage);
		}
		void draw(GLuint program, const GLchar *name, glm::mat4 trans0)
		{
			glUseProgram(program);
			VAO.enable();
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, location);
			trans = glm::rotate(trans, glm::radians(rotation_angle), rotation);
			trans = glm::scale(trans, scaling);
			glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE,
							   glm::value_ptr(trans0 * trans));

			VAO.draw(bp->usage);
		}

		void setAttribute(unsigned int index, unsigned int size, AttrFillFunc func,
						  GLenum usage)
		{
			if (index >= 1 && index <= exAttrCnt)
			{
				unsigned int bpVerNum =
					bp->getVertixBufferSize() / sizeof(float) / 3;
				const float *vbd = bp->getVertixBufferData();
				float buf[bpVerNum * size];
				for (unsigned int i = 0; i < bpVerNum; i++)
				{
					func(vbd[i * 3 + 0], vbd[i * 3 + 1], vbd[i * 3 + 2],
						 &buf[i * size]);
				}
				VAO.enable();
				VBO[index - 1].enable();
				VBO[index - 1].data(sizeof(buf), buf, usage);
				VAO.EnableAttribute(index, size, GL_FLOAT, size * sizeof(float),
									(void *)0);
			}
		}

		VertexBuffer *VBO;
	private:
		VertexArray VAO;
	};
	// Module 包含多个实体
	class Module : public Substance
	{
	public:
		void draw(GLuint program, const GLchar *name);
		void draw(GLuint program, const GLchar *name, glm::mat4 trans);
		void addSubstance(Substance &sub);
		void clear();
		void setAttribute(unsigned int index, unsigned int size,
						  void (*func)(float x, float y, float z, float *out),
						  GLenum usage);

	private:
		std::list<Substance *> subList;
	}; //

} // namespace glTool

#endif