#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "Application.h"
#include <iostream>
#include <glm.hpp>
#include <ext/matrix_transform.hpp>
#include <gtc/quaternion.hpp>


void Renderer::init() {

	shader = new Shader("vertex.glsl", "frag.glsl");
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC1_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glCullFace(GL_BACK);
    glClearColor(1, 0, 0, 1);
}




void Renderer::update(ComponentManager* manager)
{
    
    findCamera(manager);
   
    /*std::vector<entity> entities = manager->getEntities<MeshComponent>();*/
    std::vector<entity> entities = manager->getEntities();
    
    for (entity entity : entities) {
        
        if (manager->hasComponent<MeshComponent>(entity)) {
            
            meshes[manager->getComponent<MeshComponent>(entity)].push_back((manager->getComponent<TransformComponent>(entity)));
            
        }
        
        if (manager->hasComponent<PointLightComponent>(entity)) {
            lights.push_back(*manager->getComponent<PointLightComponent>(entity));
            lightTransforms.push_back(*manager->getComponent<TransformComponent>(entity));
            
        }
        if (manager->hasComponent<DirectionalLightComponent>(entity)) {
            sun = manager->getComponent<DirectionalLightComponent>(entity);
        }
    }
    
    for (const auto& mesh : meshes) {
        
        Renderer::Render(mesh.first, mesh.second);
        
    }
    
    meshes.clear();
    lights.clear();
    lightTransforms.clear();

}

void Renderer::exit()
{
}


void Renderer::Render(std::shared_ptr<MeshComponent> mesh, std::vector<std::shared_ptr<TransformComponent>> transforms)
{
    
    std::vector<glm::mat4> transform;
    transform.reserve(transforms.size());
        for (int i = 0; i < transforms.size(); i++) {
            transform.push_back(glm::mat4(1.0));
            transform[i] = glm::scale(transform[i], transforms[i]->scale);
            transform[i] = glm::translate(transform[i], transforms[i]->position);
            glm::quat q = glm::quat(transforms[i]->rotation);
            transform[i] *= glm::mat4_cast(q);
        }
        
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0); // 
    
    glBindTexture(GL_TEXTURE_2D, mesh->texture.id);

    glBindVertexArray(mesh->VAO);
    
    // vertex attributes
    
    unsigned int transformbuffer;
    glGenBuffers(1, &transformbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, transformbuffer);
    glBufferData(GL_ARRAY_BUFFER, transform.size() * sizeof(glm::mat4), &transform[0], GL_STATIC_DRAW);
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);
    glDeleteBuffers(1, &transformbuffer);
    glBindVertexArray(mesh->VAO);


    (*shader).use();
    
    
    camera->projection = glm::perspective(glm::radians(camera->fov), 800.0f / 600.0f, 0.1f, 1000.0f);
    camera->camFront = glm::normalize(camera->camFront);

    glm::mat4 view = glm::lookAt(cameraPos->position, cameraPos->position + camera->camFront, camera->camUp);
    shader->setMat4("view", view);
    shader->setMat4("projection", camera->projection);
    float lightX = sin(glfwGetTime()) * 10.0;
    float lightY = sin(glfwGetTime()) * 10.0 * cos(glfwGetTime());
    float lightZ = cos(glfwGetTime()) * 10.0;
    glm::vec3 lightPos(lightX, lightY, lightZ);
    //glm::vec3 lightColor(0.894, 0.721, 0.043);
    float r = glm::max(glm::sin(glfwGetTime()), 0.2);
    float g = glm::max(glm::cos(glfwGetTime()), 0.2);
    float b = glm::max(glm::sin(glfwGetTime()) * glm::sin(glfwGetTime()), 0.2);
    glm::vec3 lightColor(r, g, b);
    
    (*shader).setVec3("camPos", cameraPos->position);
    shader->setPointLightArray("pointLights", lights,lightTransforms);
    shader->setDirLight("dirLight", *sun);
    
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0, transform.size());
    
    glBindVertexArray(0);
    /* Swap front and back buffers */
    glfwSwapBuffers(glfwGetCurrentContext());
    glfwPollEvents();
}

void Renderer::findCamera(ComponentManager* manager){
    camera = (manager->getComponent<CameraComponent>(0));
    cameraPos = (manager->getComponent<TransformComponent>(0));
}
