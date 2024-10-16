#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"
#include <iostream>

static GLFWwindow* window;
static Scene* scene;
static bool keys[348];
static bool mouseButton[3];
static bool isDragging;
static double lastXPos, lastYPos, currXPos, currYPos,scrollX,scrollY;
int frameCount;
double previousTime;
void Application::start()
{

    /* Initialize the library */
    if (!glfwInit()) {
        return;
    }
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(2520, 1680, "Cobra", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(window,&currXPos,&currYPos);

}

void Application::addScene(Scene* newScene)
{
    scene = newScene;
    scene->start();
}

Scene* Application::swapScene(Scene* newScene)
{
    Scene* out = scene;
    scene->close();
    scene = newScene;
    
	return out;
}

Scene* Application::getScene()
{
	return scene;
	
}

void Application::run()
{
    while (!glfwWindowShouldClose(window))
    {
        previousTime = glfwGetTime();
        scene->update();
        
        //frameCount++;
        
        //if (currentTime - previousTime >= 1.0)
        //{
            
            //std::cout << getDeltaTime() << std::endl;

            //frameCount = 0;
            //previousTime = currentTime;
        //}

    }
}

bool Application::isKeyPressed(int key)
{
    return keys[key];
}

bool Application::isMouseButtonPressed(int button) {
    return mouseButton[button];
}

void Application::closeWindow() {
    scene->close();
    glfwTerminate();
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void Application::mousePositionCallback(GLFWwindow* window, double xPosition, double yPosition) {
    lastXPos = currXPos;
    lastYPos = currYPos;
    currXPos = xPosition;
    currYPos = yPosition;
    isDragging = mouseButton[0] || mouseButton[2] || mouseButton[3];
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        mouseButton[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        mouseButton[button] = false;
        isDragging = false;
    }
}

void Application::mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    scrollY -= yOffset;
    scrollX -= xOffset;
}

bool Application::cursorIsDragging() {
    return isDragging;
}

double Application::getXPos() {
    return currXPos;
}

double Application::getYPos() {
    return currYPos;
}

double Application::getXChange() {
        return currXPos - lastXPos;
}

double Application::getYChange() {
        return lastYPos - currYPos;
}

double Application::getScrollYPos() {
    return scrollY;
}
double Application::getScrollXPos() {
    return scrollX;
}

float Application::getDeltaTime() {
    return (glfwGetTime() - previousTime);
}