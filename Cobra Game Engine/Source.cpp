#pragma once


#include "Vertex.h"
#include "MeshBuilder.h"
#include "Shader.h"
#include "Renderer.h"
#include "Application.h"
#include "testscene.h"
#include "Scene.h"
#include "Component.h"
int main(void)
{

    Application::start();
    testscene test;
    Application::addScene(&test);
    Application::run();
    Application::closeWindow();
    

    return 0;
}