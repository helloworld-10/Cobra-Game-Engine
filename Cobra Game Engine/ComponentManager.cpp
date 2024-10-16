#include "ComponentManager.h"


std::vector<entity> ComponentManager::getEntities() {
	std::vector<entity> out;
	out.reserve(entityComponents.size());
	for (const auto& pair : entityComponents) {
		out.push_back(pair.first);
	}
	return out;
}
