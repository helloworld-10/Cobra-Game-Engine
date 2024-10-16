#pragma once

#include "vendor/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
class Application {
public:
	static void start();
	static void addScene(Scene* newScene);
	static Scene* swapScene(Scene* newScene);
	static Scene* getScene();
	static void run();
	static void closeWindow();
	static bool isKeyPressed(int key);
	static bool isMouseButtonPressed(int button);
	static bool cursorIsDragging();
	static double getXPos();
	static double getYPos();
	static double getXChange();
	static double getYChange();
	static double getScrollYPos();
	static double getScrollXPos();
	static float getDeltaTime();
private:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void mousePositionCallback(GLFWwindow* window, double xPosition, double yPosition);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};