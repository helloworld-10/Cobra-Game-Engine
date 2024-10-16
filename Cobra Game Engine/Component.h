#pragma once
#include "Vertex.h"
#include "Texture.h"
#include <vec3.hpp>
#include <glm.hpp>
struct Component{

};
enum ColliderType {
	Sphere,AABB,SAT
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

struct AABBColliderComponent :public ColliderComponent {
	
	glm::vec3 offset;
	glm::vec3 dimensions;
	AABBColliderComponent() {
		type = AABB;
	}
};
struct SATColliderComponent :public ColliderComponent {
	glm::vec3 offset;
	std::vector<glm::vec3>* verts;
	SATColliderComponent() {
		type = SAT;
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
	/*void calculateInertiaTensor(std::vector<Vertex>& points,glm::vec3 center) {
		std::vector<glm::vec3> coords;
		int s = points.size();
		coords.reserve(points.size());
		for (Vertex v : points) {
			coords.push_back(v.position);
		}
		glm::mat3 I = { {0,0,0},{0,0,0},{0,0,0} };
		for (glm::vec3 pos : coords) {
			I[0][0] = (mass/s)*((pos[1]-center[1]) * (pos[1] - center[1]) + (pos[2] - center[2]) * (pos[2] - center[2]));
			I[1][1] = (mass / s) *((pos[0] - center[0]) * (pos[0] - center[0]) + (pos[2] - center[2]) * (pos[2] - center[2]));
			I[2][2] = (mass / s) *((pos[0] - center[0]) * (pos[0] - center[0]) + (pos[1] - center[1]) * (pos[1] - center[1]));
			I[0][1] = (mass / s) * ((pos[0] - center[0]) * (pos[1] - center[1]));
			I[1][2] = (mass / s) * ((pos[1] - center[1]) * (pos[2] - center[2]));
			I[0][2] = (mass / s) * ((pos[0] - center[0]) * (pos[2] - center[2]));
			I[1][0] = I[0][1];
			I[2][1] = I[1][2];
			I[2][0] = I[0][2];

		}
		Iinv = glm::inverse(I);
	}*/

};

