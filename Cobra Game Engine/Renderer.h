#pragma once
#include "Shader.h"

#include "ComponentManager.h"
#include "Component.h"
#include "Behavior.h"
class Renderer : public Behavior{
	public:
		 void init() override;
		 void update(ComponentManager* manager) override;
		 void exit() override;
	private:
		Shader* shader;	
		void findCamera(ComponentManager* manager);
		void Render(std::shared_ptr<MeshComponent> mesh, std::vector<std::shared_ptr<TransformComponent>> transforms);
	std::shared_ptr<CameraComponent> camera;
	std::shared_ptr<TransformComponent> cameraPos;
		std::unordered_map<std::shared_ptr<MeshComponent>, std::vector<std::shared_ptr<TransformComponent>>> meshes;
		std::vector <PointLightComponent> lights;
		std::vector<TransformComponent> lightTransforms;
		std::shared_ptr<DirectionalLightComponent> sun;
};
