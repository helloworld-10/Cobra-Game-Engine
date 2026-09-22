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
		std::shared_ptr<ColliderComponent> c;
		std::shared_ptr<TransformComponent> t;
	};
	struct CollisionPoint {
		glm::vec3 norm;
		glm::vec3 point;
		glm::vec3 impulse;
	};
	void getPhysicsBodies(ComponentManager* manager);
	void updateBodies(ComponentManager* manager);
	void getColliderComponents(ComponentManager* manager);
	void detectCollisions(ComponentManager* manager);

	void SphereSphere(CollisionObject s1, CollisionObject s2);
	void OBBOBB(CollisionObject o1, CollisionObject o2);
	glm::vec2 projectToAxis(CollisionObject o, glm::vec3 axis);
	bool isOverlaping(CollisionObject o1, CollisionObject o2, glm::vec3 axis);
	void resolveCollisions();
	void collisionEvents();
	
	std::vector<PhysicsObject> objects;
	std::vector<CollisionObject> colliders;
	std::vector<CollisionPoint> points;

};