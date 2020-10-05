#pragma once
#include <unordered_map>
#include <memory>
#include <cassert>

#include "ComponentArray.h"

namespace ECS
{
	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			if (this->componentTypes.find(typeName) != this->componentTypes.end())
				return;

			assert(this->componentTypes.find(typeName) == this->componentTypes.end() &&
				"Registering component type more than once.");

			this->componentTypes.insert({ typeName, this->nextComponentType });
			this->componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			++this->nextComponentType;
		}

		template<typename T>
		int GetComponentType()
		{
			const char* typeName = typeid(T).name();

			assert(this->componentTypes.find(typeName) != this->componentTypes.end() &&
				"Component not registered.");

			return this->componentTypes[typeName];
		}

		template<typename T>
		void AddComponent(EntityId entity, T component)
		{
			this->GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(EntityId entity)
		{
			this->GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(EntityId entity)
		{
			return this->GetComponentArray<T>()->GetData(entity);
		}

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetAllComponents()
		{
			return this->GetComponentArray<T>();
		}

		void EntityDestroyed(EntityId entity)
		{
			for (auto const& pair : this->componentArrays)
			{
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<const char*, int> componentTypes{};
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};
		int nextComponentType{};

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			assert(this->componentArrays.find(typeName) != this->componentArrays.end() &&
				"Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(this->componentArrays[typeName]);
		}

	};
}
