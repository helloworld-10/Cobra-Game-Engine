#include "PhysicsEngine.h"
#include <ext/matrix_transform.hpp>
#include <gtc/quaternion.hpp>


void PhysicsEngine::init()
{
}

void PhysicsEngine::update(ComponentManager* manager)
{
	//(std::static_pointer_cast<SATColliderComponent>(colliders[1].c))->verts->size()
	getPhysicsBodies(manager);
	updateBodies(manager);
	
	getColliderComponents(manager);
	
	detectCollisions(manager);
	
}

void PhysicsEngine::exit()
{
}

void PhysicsEngine::getPhysicsBodies(ComponentManager* manager) {
	objects.clear();
	std::vector<entity> physicsEntities = manager->getEntities<RigidBodyComponent>();
	objects.reserve(physicsEntities.size());
	for (entity e : physicsEntities) {
		PhysicsObject p;
		p.rb = (manager->getComponent<RigidBodyComponent>(e));
		p.t = (manager->getComponent<TransformComponent>(e));
		objects.push_back(p);
	}
}

void PhysicsEngine::updateBodies(ComponentManager* manager)
{
	float dt = Application::getDeltaTime();
	for (PhysicsObject p : objects) {
		dt = Application::getDeltaTime();
		p.rb->velocity += p.rb->force/p.rb->mass * dt;
		p.t->position += p.rb->velocity * dt;
		
		glm::mat4 r = p.t->rotation;
		float e = glm::dot(r[0], r[1]);
		glm::vec3 ox = glm::normalize(r[0] - (e / 2) * r[1]);
		glm::vec3 oy = glm::normalize(r[1] - (e / 2) * r[0]);
		glm::vec3 oz = glm::normalize(glm::cross(ox, oy));
		p.t->rotation = glm::mat3(ox, oy, oz);

		p.rb->L += p.rb->torque*dt;
		glm::vec3 rotVel = p.t->rotation *p.rb->Iinv *glm::transpose(p.t->rotation)*p.rb->L;
		p.t->rotation += glm::mat3(glm::cross(rotVel, p.t->rotation[0]), glm::cross(rotVel, p.t->rotation[1]), glm::cross(rotVel, p.t->rotation[2])) * dt;
		
		p.rb->force = { 0,0,0 };
		p.rb->torque = { 0,0,0 };
		
	}
	
}

void PhysicsEngine::getColliderComponents(ComponentManager* manager) {
	colliders.clear();
	std::vector<entity> colliderEntities = manager->getEntities<ColliderComponent>();
	colliders.reserve(colliderEntities.size());
	for (entity e : colliderEntities) {
		CollisionObject sp;
		sp.c = (manager->getComponent<ColliderComponent>(e));
		sp.t = (manager->getComponent<TransformComponent>(e));
		
		if (sp.c->type == OBB) {
			
			//glm::mat4 transform(1.0);
			////glm::scale(transform, sp.t->scale);
			//glm::translate(transform,sp.t->position);
			//glm::quat q = glm::quat(sp.t->rotation);
			//	transform *= glm::mat4_cast(q);
				for (int i = 0; i < 8; i++) {
					sp.c->obb.verts[i] = sp.t->position+sp.t->rotation*sp.c->obb.vertsPoints[i];
				}

			
			colliders.push_back(sp);
			
		}
	}
}

void PhysicsEngine::detectCollisions(ComponentManager* manager)
{

	for (int i = 0; i < colliders.size(); i++) {
		for (int j = i + 1; j < colliders.size(); j++) {
			if (colliders[i].c->type == Sphere && colliders[j].c->type == Sphere) {
				CollisionObject s1;
				CollisionObject s2;
				s1.c = (colliders[i].c);
				s1.t = colliders[i].t;
				s2.c = (colliders[j].c);
				s2.t = colliders[j].t;
				SphereSphere(s1, s2);
			}
			if (colliders[i].c->type == OBB && colliders[j].c->type == OBB) {
				CollisionObject s1;
				CollisionObject s2;
				s1.c = (colliders[i].c);
				s1.t = colliders[i].t;
				s2.c = (colliders[j].c);
				s2.t = colliders[j].t;

				OBBOBB(s1, s2);
			}

		}

	}
}

void PhysicsEngine::SphereSphere(CollisionObject s1, CollisionObject s2) {
	float sum = s1.c->sphere.radius + s2.c->sphere.radius;
	if (glm::length(s1.t->position - s2.t->position) <= sum) {
		glm::vec3 norm = glm::normalize(s1.t->position - s2.t->position);
		float depth = sum - glm::length(s1.t->position - s2.t->position);
		s1.t->position += norm * depth / 2.0f;
		s2.t->position += -norm * depth / 2.0f;

	}

}

void PhysicsEngine::OBBOBB(CollisionObject o1, CollisionObject o2)
{
	std::vector<glm::vec3> axes;
	axes.reserve(15);
	axes.push_back(o1.c->obb.verts[0]);
	axes.push_back(o1.c->obb.verts[1]);
	axes.push_back(o1.c->obb.verts[2]);
	axes.push_back(o2.c->obb.verts[0]);
	axes.push_back(o2.c->obb.verts[1]);
	axes.push_back(o2.c->obb.verts[2]);
	axes.push_back(glm::cross(o1.c->obb.verts[0], o2.c->obb.verts[0]));
	axes.push_back(glm::cross(o1.c->obb.verts[0], o2.c->obb.verts[1]));
	axes.push_back(glm::cross(o1.c->obb.verts[0], o2.c->obb.verts[2]));
	axes.push_back(glm::cross(o1.c->obb.verts[1], o2.c->obb.verts[0]));
	axes.push_back(glm::cross(o1.c->obb.verts[1], o2.c->obb.verts[1]));
	axes.push_back(glm::cross(o1.c->obb.verts[1], o2.c->obb.verts[2]));
	axes.push_back(glm::cross(o1.c->obb.verts[2], o2.c->obb.verts[0]));
	axes.push_back(glm::cross(o1.c->obb.verts[2], o2.c->obb.verts[1]));
	axes.push_back(glm::cross(o1.c->obb.verts[2], o2.c->obb.verts[2]));
	for (const auto& axis : axes) {
		if (isOverlaping(o1, o2, glm::normalize(axis))) {
			return;
		}
	}


}

glm::vec2 PhysicsEngine::projectToAxis(CollisionObject collider, glm::vec3 axis)
{
	float min = 1000000;
	float max = -100000;
	for (int i = 0; i < 8; i++) {
		float p = glm::dot(collider.c->obb.verts[i], axis);
		if (min > p) { min = p; };
		if (max < p) { max = p; };
	}
	return { min,max };
}

bool PhysicsEngine::isOverlaping(CollisionObject o1, CollisionObject o2, glm::vec3 axis) {
	glm::vec2 p1 = projectToAxis(o1, axis);
	glm::vec2 p2 = projectToAxis(o2, axis);
	return p1.x >= p2.y || p2.x >= p1.y;
}



void PhysicsEngine::resolveCollisions()
{

}

void PhysicsEngine::collisionEvents()
{
}
