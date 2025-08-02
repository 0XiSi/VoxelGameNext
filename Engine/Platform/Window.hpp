#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {
	enum class Key { F5 = GLFW_KEY_F5 };

	class Window {
	public:
		Window(int, int, const char*) {}
		bool init() { return glfwInit() && (m_win = glfwCreateWindow(1280, 720, "VoxelGame", nullptr, nullptr)); }
		bool shouldClose() { return glfwWindowShouldClose(m_win); }
		void pollEvents() { glfwPollEvents(); }
		void swapBuffers() { glfwSwapBuffers(m_win); }
		bool isKeyPressed(Key k) { return glfwGetKey(m_win, static_cast<int>(k)) == GLFW_PRESS; }
	private:
		GLFWwindow* m_win = nullptr;
	};
}

class Window {
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialize();
	GLint getBufferWidth() const { return bufferWidth; }
	GLint getBufferHeight() const { return bufferHeight; }
	bool shouldClose() { return glfwWindowShouldClose(mainWindow); }
	void swapBuffer() { glfwSwapBuffers(mainWindow); }
	GLFWwindow* getGLFWWindow() const { return mainWindow; }

	bool* getKeys() {
		return keys;
	}
	GLfloat getXChange();
	GLfloat getYChange();

	~Window();
private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallBacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mods);
	static void handleMouseKeys(GLFWwindow* window, int button, int action, int mods);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};