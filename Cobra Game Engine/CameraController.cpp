#include "CameraController.h"
#include "Application.h"
#include "Component.h"

float yaw = -90.0f;
float pitch = 0;
float roll = 0;

float prevChangeY = 0;
float prevChangeX = 0;
float prevScroll = 0;
void CameraController::init()
{
    
}

void CameraController::update(ComponentManager* manager)
{
    std::shared_ptr<TransformComponent> t;
    std::shared_ptr<CameraComponent> camera;
    
    for (entity entity : manager->getEntities()) {
        if (manager->hasComponent<CameraComponent>(entity)) {
            camera = manager->getComponent<CameraComponent>(entity);
            t = manager->getComponent<TransformComponent>(entity);
        }
    }
    glm::vec3 p = t->position;
    std::cout << std::to_string(p.x) + " "+ std::to_string(p.y) + " " + std::to_string(p.z) << std::endl;
    float speed = 0.2f;
    glm::vec3 right = { 1,0,0 };
    glm::vec3 up = { 0,1,0 };
    glm::vec3 forward = { 0,0,1 };
    if (Application::isKeyPressed(GLFW_KEY_W)) {
        t->position += speed * camera->camFront;
    }
    if (Application::isKeyPressed(GLFW_KEY_A)) {
        t->position -= speed * glm::cross(camera->camFront, camera->camUp);
    }
    if (Application::isKeyPressed(GLFW_KEY_S)) {
        t->position -= speed * camera->camFront;
    }
    if (Application::isKeyPressed(GLFW_KEY_D)) {
        t->position += speed * glm::cross(camera->camFront, camera->camUp);
    }
    if (Application::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        t->position -= speed * up;
    }
    if (Application::isKeyPressed(GLFW_KEY_SPACE)) {
        t->position += speed * up;
    }
    if (Application::isKeyPressed(GLFW_KEY_ESCAPE)) {
        Application::closeWindow();
    }
    if (Application::isKeyPressed(GLFW_KEY_E)) {
        //manager->getComponent<RigidBodyComponent>(1)->velocity={ 0, 1, 0 };
        manager->getComponent<RigidBodyComponent>(1)->addTorque( {-1, 5, 2});

    }
    if (Application::isKeyPressed(GLFW_KEY_R)) {
        //manager->getComponent<RigidBodyComponent>(1)->velocity={ 0, 1, 0 };
        manager->getComponent<RigidBodyComponent>(1)->addTorque({ 1, -5, -2 });

    }

    camera->camFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera->camFront.y = sin(glm::radians(pitch));
    camera->camFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    if (Application::getXChange() != prevChangeX) {
        yaw += 0.09 * Application::getXChange();
        prevChangeX = Application::getXChange();
    }
    if (Application::getYChange() != prevChangeY) {
        pitch += 0.09 * Application::getYChange();
        prevChangeY = Application::getYChange();
    }

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    if (Application::getScrollYPos() != prevScroll) {
        camera->fov -= (float)Application::getScrollYPos() / 20.0f;
        prevScroll = Application::getScrollYPos();
    }
    if (camera->fov < 1.0f)
        camera->fov = 1.0f;
    if (camera->fov > 90.0f)
        camera->fov = 45.0f;
}

void CameraController::exit()
{
}
