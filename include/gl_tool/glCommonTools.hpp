#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
namespace glTool {

GLFWwindow *glfwGladInitCreateWindow(int width, int height, const char *title) {
	// glfw 初始化
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw 创建窗口
	// --------------------
	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	// glad 初始化
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return NULL;
	}
	return window;
}

inline void showFPS() {
	static int c = 0, fc = 0;
	fc++;
	if (c == int(glfwGetTime()) % 2)
		return;
	std::cout << "\r         \rfps : " << fc;
	fc = 0;
	c = 1 - c;
}
inline void controlFPS(int fps = 0) {
	static int FPSSet=-1, lastF = -1, temp;
	if (fps != 0)
		FPSSet = fps;
	else {
		if (FPSSet < 0)
			return;
		do { //这里随便选一个大一点的常数
			temp = int(glfwGetTime() * FPSSet) % 32;
		} while (lastF == temp);
		lastF = temp;
	}
} 
} // namespace glTool