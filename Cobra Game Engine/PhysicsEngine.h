#pragma once
#include "Behavior.h"
#include "Application.h"
class PhysicsEngine : public Behavior {
	void init() override;
	void update(ComponentManager* manager) override;
	void exit() override;
	
private:
	struct PhysicsObject {
		std::shared_ptr<RigidBodyComponent> rb;
		std::shared_ptr<TransformComponent> t;
	};
	struct CollisionObject {
		entity e;
		std::shared_ptr<ColliderComponent> c;
		std::shared_ptr<TransformComponent> t;
	};
	struct SphereCollisionObject {
		entity e;
		std::shared_ptr<SphereColliderComponent> c;
		std::shared_ptr<TransformComponent> t;
	};
	struct AABBCollisionObject {
		entity e;
		std::shared_ptr<AABBColliderComponent> c;
		std::shared_ptr<TransformComponent> t;
	};
	struct SATCollisionObject {
		entity e;
		std::shared_ptr<SATColliderComponent> c;
		std::shared_ptr<TransformComponent> t;
	};
	struct CollResponse {
		entity e1, e2;
		glm::vec3 normal;
		float depth;
		std::vector<glm::vec3> cPoints;
	};
	void getPhysicsBodies(ComponentManager* manager);
	void updateBodies(ComponentManager* manager);
	void getColliderComponents(ComponentManager* manager);
	void detectCollisions(ComponentManager* manager);

	void SphereSphere(SphereCollisionObject s1, SphereCollisionObject s2);
	void resolveCollisions(CollResponse c);
	void collisionEvents();
	
	std::vector<PhysicsObject> objects;
	std::vector<CollisionObject> colliders;

};