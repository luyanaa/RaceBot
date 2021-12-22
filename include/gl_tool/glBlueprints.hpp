
#pragma once
#include "gl_tool/glModule.h"
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;
namespace glTool
{
	class BallBp : public Blueprint
	{
	public:
		BallBp(int n)
		{
			usage = GL_TRIANGLES;
			if (n < 1)
				n = 1;

			// 八象限 3顶点
			vbs = 8 * 3 * (n + 2) * (n + 1) / 2;
			ves = 8 * 3 * n * n;

			vbd = new float[vbs];
			ved = new unsigned int[ves];

			int x = 0, y = 0;
			// 八个象限
			bool reverse[8] = {0, 1, 1, 0, 1, 0, 0, 1};
			for (int k = 0; k < 8; k++)
			{
				// n 层
				for (int i = 0; i <= n; i++)
				{
					for (int j = 0; j <= i; j++)
					{
						vbd[x * 3 + 0] = (k & 1 ? -1 : 1) * (1.0 - i * 1.0 / n);
						vbd[x * 3 + 1] = (k & 2 ? -1 : 1) * ((j - i) * 1.0 / n);
						vbd[x * 3 + 2] = (k & 4 ? -1 : 1) * (j * 1.0 / n);
						float m = sqrtf(vbd[x * 3 + 0] * vbd[x * 3 + 0] +
										vbd[x * 3 + 1] * vbd[x * 3 + 1] +
										vbd[x * 3 + 2] * vbd[x * 3 + 2]);
						vbd[x * 3 + 0] /= m;
						vbd[x * 3 + 1] /= m;
						vbd[x * 3 + 2] /= m;
						if (i < n)
						{
							ved[y * 3 + 0] = x;
							ved[y * 3 + 1] = x + i + 1 + (reverse[k] ? 0 : 1);
							ved[y * 3 + 2] = x + i + 1 + (reverse[k] ? 1 : 0);
							++y;
							if (j < i)
							{
								ved[y * 3 + 0] = x + (reverse[k] ? 1 : 0);
								ved[y * 3 + 1] = x + (reverse[k] ? 0 : 1);
								ved[y * 3 + 2] = x + i + 2;
								++y;
							}
						}
						++x;
					}
				}
			}
			VBO.data(getVertixBufferSize(), vbd, GL_STATIC_DRAW);
			EBO.data(getVertixElementSize(), ved, GL_STATIC_DRAW);
		}
		~BallBp()
		{
			delete[] vbd;
			delete[] ved;
		}
		const float *getVertixBufferData() const { return vbd; }
		const unsigned int *getVertixElementData() const { return ved; }
		int getVertixBufferSize() const { return vbs * sizeof(float); }
		int getVertixElementSize() const { return ves * sizeof(unsigned int); }

	private:
		int vbs;
		int ves;

		float *vbd;
		unsigned int *ved;
	}; // allBp
	class CubeBp : public Blueprint
	{
	public:
		CubeBp()
		{
			usage = GL_TRIANGLES;
			VBO.data(sizeof(vertices), vertices, GL_STATIC_DRAW);
			EBO.data(sizeof(indices), indices, GL_STATIC_DRAW);
		}
		const float *getVertixBufferData() const { return vertices; }
		const unsigned int *getVertixElementData() const { return indices; }
		int getVertixBufferSize() const { return 24 * sizeof(float); };
		int getVertixElementSize() const { return 36 * sizeof(unsigned int); };

	private:
		float vertices[24] = {
			1,
			1,
			-1,
			1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			1,
			-1,
			1,
			1,
			1,
			1,
			-1,
			1,
			-1,
			-1,
			1,
			-1,
			1,
			1,
		};
		unsigned int indices[36] = {
			4, 3, 7, // 前三面
			4, 0, 3, //
			4, 1, 0, //
			4, 5, 1, //
			4, 6, 5, //
			4, 7, 6, //

			2, 7, 3, // 后三面
			2, 3, 0, //
			2, 0, 1, //
			2, 1, 5, //
			2, 5, 6, //
			2, 6, 7, //
		};
	}; // Cube
	class CircleBp : public Blueprint
	{

	public:
		CircleBp(int n)
		{
			usage = GL_LINE_STRIP;
			if (n < 1)
				n = 1;

			vbs = 3 * n;
			ves = n + 1;

			vbd = new float[vbs];
			ved = new unsigned int[ves];

			for (int i = 0; i < n; i++)
			{
				vbd[i * 3 + 0] = sin(2 * atan2(0, -1) * i / n);
				vbd[i * 3 + 1] = 0;
				vbd[i * 3 + 2] = cos(2 * atan2(0, -1) * i / n);
				ved[i] = i;
			}
			ved[n] = 0;
			VBO.data(getVertixBufferSize(), vbd, GL_STATIC_DRAW);
			EBO.data(getVertixElementSize(), ved, GL_STATIC_DRAW);
		}
		~CircleBp()
		{
			delete[] vbd;
			delete[] ved;
		}
		const float *getVertixBufferData() const { return vbd; }
		const unsigned int *getVertixElementData() const { return ved; }
		int getVertixBufferSize() const { return vbs * sizeof(float); }
		int getVertixElementSize() const { return ves * sizeof(unsigned int); }

	private:
		int vbs;
		int ves;

		float *vbd;
		unsigned int *ved;
	};

} // namespace glTool