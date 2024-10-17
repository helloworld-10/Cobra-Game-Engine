#pragma once
#include "Vertex.h"
#include "Texture.h"
#include <vec3.hpp>
#include <glm.hpp>
struct Component{

};
enum ColliderType {
	Sphere,OBB
};

struct MeshComponent:public Component {
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	int indicesSize;
	Texture texture;
	unsigned int VAO, VBO, EBO;
};


struct TransformComponent :public Component {
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat3 rotation = { {1,0,0},{0,1,0},{0,0,1} };
};


struct CameraComponent :public Component {
	glm::vec3 camFront;
	glm::vec3 camUp;
	glm::vec3 camRight;
	glm::mat4 projection;
	float fov = 45;
};

struct DirectionalLightComponent :public Component {
	glm::vec3 direction;
	glm::vec3 color;
};

struct PointLightComponent :public Component {
	glm::vec3 color;
	glm::vec3 attenuation;
};

struct SpotlightComponent :public Component {
	glm::vec3  direction;
	glm::vec3 color;
	float innerCutoff;
	float outerCutoff;
};

struct ColliderComponent :Component{
	ColliderType type;
	bool isStatic = false;
};
struct SphereColliderComponent :public ColliderComponent {
	
	glm::vec3 offset;
	float radius;
	SphereColliderComponent() {
		type = Sphere;
	}
};

struct OBBColliderComponent :public ColliderComponent {
	glm::vec3 offset;
	glm::vec3 halfSize;
	glm::vec3 vertsPoints[8];
	glm::vec3 verts[8];
	OBBColliderComponent(glm::vec3 hlfsze) {
		type = OBB;
		halfSize = hlfsze;
		vertsPoints[0] = {-halfSize.x, -halfSize.y, -halfSize.z};
		vertsPoints[1] = {halfSize.x, -halfSize.y, +halfSize.z};
		vertsPoints[2] = { -halfSize.x, +halfSize.y, -halfSize.z};
		vertsPoints[3] = {-halfSize.x, +halfSize.y, +halfSize.z};
		vertsPoints[4] = {+halfSize.x, -halfSize.y, -halfSize.z};
		vertsPoints[5] = {+halfSize.x, -halfSize.y, +halfSize.z};
		vertsPoints[6] = {+halfSize.x, +halfSize.y, -halfSize.z};
		vertsPoints[7] = {+halfSize.x, +halfSize.y, +halfSize.z};
	}
};

struct RigidBodyComponent :public Component {
	glm::vec3 velocity = {0,0,0};
	glm::vec3 force = {0,0,0};
	glm::vec3 torque = { 0,0,0 };
	glm::vec3 L = { 0,0,0 };
	glm::mat3 Iinv = { {.1,0,0} ,{0,.1,0}, {0,0,.1} };
	float mass = 1;
	glm::vec3 COMoffset = {0,0,0};

	inline void addForce(glm::vec3 f) {
		force += f;
	};
	inline void addTorque(glm::vec3 t) {
		torque += t;
	};

};

