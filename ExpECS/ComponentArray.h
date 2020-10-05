#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace ECS
{
	// virtual baseclass to allow having arrays
	// of components with varied types. 
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(EntityId entity) = 0;
	};
	
	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(EntityId id, T component)
		{
			size_t newIndex = this->sizeOfEntries;

			this->componentArray[newIndex] = component;
			
			//bookkeeping entities <-> indices
			this->entityToIndexMap[id] = newIndex;

			this->EntCompPairing.push_back({ id, newIndex });

			++this->sizeOfEntries;
		}

		void RemoveData(EntityId id)
		{
			if (FindEntity(id))
			{
				size_t indexLast = this->sizeOfEntries - 1;

				int from = INT_MIN; 
				int to = INT_MIN;
				for (int i = 0; i < this->entityComponentPairs.size(); i++)
				{
					ECPair& pair = this->entityComponentPairs[i];

					// find index in mapping vector to the entity 
					// corresponding to last index in componentArray
					if (indexLast == pair.arrayIndex)
						from = i;

					// find index in mapping vector to the entity
					// which is no longer valid 
					if (pair.id == id)
						to = i;
	
					if (from >= 0 && to >= 0)
						break;
				}

				// element at 'to' maps invalid entity + component pair 
				size_t indexRemove = this->entityComponentPairs[to].arrayIndex;
				if (indexLast != indexRemove)
				{
					// To ensure tighly packed data:
					// replace component of removed entity with the last one in componentArray
					this->componentArray[indexRemove] = this->componentArray[indexLast];
				}
			
				// bookkeping of entityComponentPairs:
				// Element at 'to' has correct arraIndex but wrong EnsätityId.
				// Replace with EntityId corresp. to moved component.  
				this->entityComponentPairs[to].id = this->entityComponentPairs[from].id;

				// Last element now invalid, erase.
				this->entityComponentPairs.erase(EntCompPairing.begin() + from);

				// This ComponentArray one element less; reduce size.
				--this->sizeOfEntries;
			}
		}

		T& GetData(EntityId id)
		{
			size_t index = this->entityToIndexMap[id];
			return componentArray[index];
		}

		void EntityDestroyed(EntityId id) override
		{
			if (FindEntity(id))
			{
				this->RemoveData(id);
			}
		}

	private:
		bool FindEntity(EntityId id)
		{
			return this->entityToIndexMap.find(id) != this->entityToIndexMap.end();
		}

		std::array<T, MAX_ENTITIES> componentArray;

		struct ECPair
		{
			EntityId id;
			size_t arrayIndex;
		};

		std::vector<ECPair> entityComponentPairs;
		std::unordered_map<EntityId, size_t> entityToIndexMap;
		size_t sizeOfEntries;
	};
}