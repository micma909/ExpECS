#include <stdio.h>
#include <iostream>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Common.h"
#include "ComponentArray.h"
#include "linalg.h"

using namespace linalg::aliases;

struct SimpleTransformComponent
{
	float3 pos;
};

int main(int argc, const char* argv[])
{

	std::unique_ptr<ECS::EntityManager> entityManager = std::make_unique<ECS::EntityManager>();	
	
	std::vector<ECS::Entity*> entities(10);

	entityManager->InitEntities(entities);

	for (auto& entity : entities)
	{
		SimpleTransformComponent transform;
		transform.pos = float3(0, 0, 0);

		entity->AddComponent(transform);
	}


	for (auto& entity : entities)
	{
		SimpleTransformComponent& trans = entity->GetComponent<SimpleTransformComponent>();
		std::cout << trans.pos[0] << " " << trans.pos[1] << " " << trans.pos[2] << std::endl;
	}


	for (auto& entity : entities)
	{
		SimpleTransformComponent& trans = entity->GetComponent<SimpleTransformComponent>();
		trans.pos += float3(1, 1, 1);
	}

	for (auto& entity : entities)
	{
		SimpleTransformComponent& trans = entity->GetComponent<SimpleTransformComponent>();
		std::cout << trans.pos[0] << " " << trans.pos[1] << " " << trans.pos[2] << std::endl;
	}
	
	delete entities[3];
	delete entities[0];

	//for (int i = 0; i < entities.size(); i++)
	//{
	//	delete entities[i];
	//}

}