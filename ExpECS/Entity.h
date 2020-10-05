#pragma once

#include "Common.h"
#include "ComponentArray.h"
#include "ComponentManager.h"

namespace ECS
{	
	// An entity is created through Entity Manager ONLY!
	// There it receives a ref to component manager.
	class Entity
	{
	public:
		

		template<typename T>
		void AddComponent(T component)
		{
			// Register in, and Add component to ComponentManager
			this->componentManager->RegisterComponent<T>();
			this->componentManager->AddComponent<T>(id, component);
		}

		template<typename T>
		T& GetComponent()
		{
			return this->componentManager->GetComponent<T>(id);
		}

		~Entity()
		{
			this->componentManager->EntityDestroyed(id);
		}

	protected:
		friend class EntityManager;
		Entity() : componentManager(nullptr), id(-1)
		{};

	
		EntityId id;

	private: 
		std::shared_ptr<ComponentManager> componentManager;
	};

}