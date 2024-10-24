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
	glm::vec3 position = {0,0,0};
	glm::vec3 scale = {1,1,1};
	glm::mat3 rotation = { {1.0f,0,0},{0,1.0f,0},{0,0,1.0f} };
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

struct SphereCollider {
	float radius = 0;
};
struct OBBCollider {
	glm::vec3 halfSize = {0,0,0};
	glm::vec3 vertsPoints[8] = {};
	glm::vec3 verts[8] = {};
};
struct ColliderComponent :Component{
	ColliderType type = OBB;
	bool isStatic = false;
	glm::vec3 offset = {};
	union {
		SphereCollider sphere;
		OBBCollider obb;
	};
	ColliderComponent(glm::vec3 hlfsze) {
		type = OBB;
		obb.halfSize = hlfsze;
		obb.vertsPoints[0] = { -obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z };
		obb.vertsPoints[1] = { obb.halfSize.x, -obb.halfSize.y, +obb.halfSize.z };
		obb.vertsPoints[2] = { -obb.halfSize.x, +obb.halfSize.y, -obb.halfSize.z };
		obb.vertsPoints[3] = { -obb.halfSize.x, +obb.halfSize.y, +obb.halfSize.z };
		obb.vertsPoints[4] = { +obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z };
		obb.vertsPoints[5] = { +obb.halfSize.x, -obb.halfSize.y, +obb.halfSize.z };
		obb.vertsPoints[6] = { +obb.halfSize.x, +obb.halfSize.y, -obb.halfSize.z };
		obb.vertsPoints[7] = { +obb.halfSize.x, +obb.halfSize.y, +obb.halfSize.z };
	}
	ColliderComponent(float r) {
		type = Sphere;
		sphere.radius = r;
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

