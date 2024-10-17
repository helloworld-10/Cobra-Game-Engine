#include "PhysicsEngine.h"


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
		sp.e = e;
		sp.c = (manager->getComponent<ColliderComponent>(e));
		sp.t = (manager->getComponent<TransformComponent>(e));
		colliders.push_back(sp);
		if (sp.c->type == OBB) {
			for (int i = 0; i < 8; i++) {	
				(std::static_pointer_cast<OBBColliderComponent>(sp.c))->verts[i] = sp.t->position +
					(std::static_pointer_cast<OBBColliderComponent>(sp.c))->vertsPoints[i];
			}
		}
	}
}

void PhysicsEngine::detectCollisions(ComponentManager* manager)
{
	
	for (int i = 0; i < colliders.size(); i++) {
		for (int j = i+1; j < colliders.size(); j++) {
			if (colliders[i].c->type == Sphere && colliders[j].c->type == Sphere) {
				SphereCollisionObject s1;
				SphereCollisionObject s2;
				s1.c = std::static_pointer_cast<SphereColliderComponent>(colliders[i].c);
				s1.t = colliders[i].t;
				s2.c = std::static_pointer_cast<SphereColliderComponent>(colliders[j].c);
				s2.t = colliders[j].t;
				SphereSphere(s1, s2);
			}
			if (colliders[i].c->type == OBB && colliders[j].c->type == OBB) {
				OBBCollisionObject s1;
				OBBCollisionObject s2;
				s1.o = std::static_pointer_cast<OBBColliderComponent>(colliders[i].c);
				s1.t = colliders[i].t;
				s2.o = std::static_pointer_cast<OBBColliderComponent>(colliders[j].c);
				s2.t = colliders[j].t;
				OBBOBB(s1, s2);
			}
			
		}
		
	}
}

void PhysicsEngine::SphereSphere(SphereCollisionObject s1, SphereCollisionObject s2) {
	float sum = s1.c->radius + s2.c->radius;
	if (glm::length(s1.t->position - s2.t->position) <= sum) {
		glm::vec3 norm = glm::normalize(s1.t->position - s2.t->position);
		float depth = sum - glm::length(s1.t->position - s2.t->position);
		s1.t->position += norm * depth/2.0f;
		s2.t->position += -norm * depth/2.0f;

	}
	
}

void PhysicsEngine::OBBOBB(OBBCollisionObject o1, OBBCollisionObject o2)
{
	std::vector<glm::vec3> axes;
	axes.reserve(15);
	axes.push_back(o1.o->verts[0]);
	axes.push_back(o1.o->verts[1]);
	axes.push_back(o1.o->verts[2]);
	axes.push_back(o2.o->verts[0]);
	axes.push_back(o2.o->verts[1]);
	axes.push_back(o2.o->verts[2]);
	axes.push_back(glm::cross(o1.o->verts[0], o2.o->verts[0]));
	axes.push_back(glm::cross(o1.o->verts[0], o2.o->verts[1]));
	axes.push_back(glm::cross(o1.o->verts[0], o2.o->verts[2]));
	axes.push_back(glm::cross(o1.o->verts[1], o2.o->verts[0]));
	axes.push_back(glm::cross(o1.o->verts[1], o2.o->verts[1]));
	axes.push_back(glm::cross(o1.o->verts[1], o2.o->verts[2]));
	axes.push_back(glm::cross(o1.o->verts[2], o2.o->verts[0]));
	axes.push_back(glm::cross(o1.o->verts[2], o2.o->verts[1]));
	axes.push_back(glm::cross(o1.o->verts[2], o2.o->verts[2]));
	for (const auto& axis : axes) {
		if (isOverlaping(o1, o2, glm::normalize(axis))) {
			std::cout << "hlp\n";
			return;
		}
	}




}

glm::vec2 PhysicsEngine::projectToAxis(OBBCollisionObject collider, glm::vec3 axis)
{
	float min = 1000000;
	float max = -100000;
	for (int i = 0; i < 8; i++) {
		float p = glm::dot(collider.o->verts[i], axis);
		if (min > p) { min = p; };
		if (max < p) { max = p; };
	}
	return { min,max };
}

bool PhysicsEngine::isOverlaping(OBBCollisionObject o1, OBBCollisionObject o2, glm::vec3 axis) {
	glm::vec2 p1 = projectToAxis(o1, axis);
	glm::vec2 p2 = projectToAxis(o2, axis);
	return p1.x >= p2.y || p2.x >= p1.y;
}



void PhysicsEngine::resolveCollisions(CollResponse c)
{

}

void PhysicsEngine::collisionEvents()
{
}
