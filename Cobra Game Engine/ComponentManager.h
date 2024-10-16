#pragma once
#include <unordered_map>
#include <string>
#include <typeindex>
#include <iostream>
#include "Component.h"
using bitset = int;
using entity = int;
class ComponentManager {
public:
	entity entityIndex;
	entity generateEntity() {
		return entityIndex++;
	};


	template<typename T>
	inline void addComponent(entity entity, std::shared_ptr<T> component)
	{
		if (!ComponentManager::hasComponent<T>(entity)) {
			int id = componentID[typeid(T).name()];
			if (!components[id]) {
				components[id] = std::make_unique<std::unordered_map<int, std::shared_ptr<Component>>>();
			}
			(*components[id])[entity] = std::static_pointer_cast<Component>(component);
			entityComponents[entity] += 1 << id;
		}
		else {
			return;
		}
	}



	template<typename... T>
	inline void registerComponent(T... t)
	{
		componentIndex = 1;
		std::vector<std::type_index> args = { typeid(t)... };
		for (const auto& p : args) {
			componentID.emplace(p.name(), componentIndex);
			componentIndex++;
		}
		components.reserve(args.size()+1);
		components.resize(args.size()+1);
	}



	template<typename T>
	inline void removeComponent(entity entity)
	{
		if (ComponentManager::hasComponent<T>(entity)) {
			int id = componentID[typeid(T).name()];
			components[id]->erase(entity);
			entityComponents[entity] -= 1 << id;
		}
	}
	


	template<typename T>
	inline std::shared_ptr<T> getComponent(entity entity)
	{
		int id = componentID[typeid(T).name()];
		auto it = components[id]->find(entity);
		if (it != components[id]->end()) {
			return std::static_pointer_cast<T>(it->second);
		}
		else {
			std::shared_ptr<T> c = nullptr;
			return c;
		}
	}



	template<typename ...T>
	inline std::tuple<T...> getComponents(entity entity)
	{
		return std::make_tuple(getComponent<T>(entity)...);
	}



	template<typename T>
	inline bool hasComponent(entity entity)
	{
		int id = componentID[typeid(T).name()];
		bitset bits = entityComponents[entity];
		return ((bits & 1<<id) > 0) ? true : false;
	}

	template<typename T>
	inline std::vector<entity> getEntities()
	{
		int id = componentID[typeid(T).name()];
		std::vector<entity> out;
		out.reserve((*components[id]).size());
		for (const auto& pair : *components[id]) {
			out.push_back(pair.first);
		}
		return out;
	}
	std::vector<entity> getEntities();

	int componentIndex = 1;
	std::unordered_map<entity,bitset> entityComponents;
	std::vector<std::unique_ptr<std::unordered_map<entity, std::shared_ptr<Component>>>> components;
	//TODO: implement sparse set, make vector of sparse sets to store components, use entities as keys for the sparseset
	std::unordered_map<std::string, int> componentID;
};