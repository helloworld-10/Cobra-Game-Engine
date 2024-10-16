#pragma once
#include "ComponentManager.h"
#include "Behavior.h"
class Scene {

public:
	virtual void start() = 0;
	virtual void update() = 0;
	virtual void close() = 0;
	ComponentManager manager;
	std::vector<Behavior*> behaviors;
};