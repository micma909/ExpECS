#pragma once
#include "Common.h"
#include "Entity.h"
#include "ComponentManager.h"

#include <queue> // might change this 

namespace ECS 
{

	class EntityManager
	{
	public:
		EntityManager()
		{
			componentManager = std::make_shared<ComponentManager>();

			for (EntityId ent = 0; ent < MAX_ENTITIES; ++ent)
			{
				this->availableEntitiesQ.push(ent);
			}
		}

		void InitEntities(Entity* entity)
		{
			entity = new Entity();
			entity->id = this->AssignEntityID();
			entity->componentManager = this->componentManager;
		}

		void InitEntities(std::vector<Entity*>& entites)
		{
			for(int i = 0; i < entites.size(); i++)
			{ 
				entites[i] = new Entity();
				entites[i]->id = this->AssignEntityID();
				entites[i]->componentManager = this->componentManager;
			}
		}

	private:
		EntityId AssignEntityID()
		{
			EntityId ID = this->availableEntitiesQ.front();

			this->availableEntitiesQ.pop();
			this->nrEntitiesAlive++;

			return ID;
		}

		std::shared_ptr<ComponentManager> componentManager;
		std::queue<EntityId> availableEntitiesQ{};
		uint32_t nrEntitiesAlive{};
	};
}