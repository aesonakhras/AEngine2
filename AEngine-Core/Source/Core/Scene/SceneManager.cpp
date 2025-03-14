#include "SceneManager.h"
#include "Core/Debug.h"

#include "Core/Events/EventManager.h"
#include "Core/Events/DestroyEntityEvent.h"

#include "Core/Systems/SystemLocator.h"
#include "Core/Systems/TransformSystem.h"

using namespace AE::Core;

entt::entity SceneManager::CreateEntity() {
	entt::entity newEntity = Registry.create();

	//TODO_E: Code to handle entity creation failure

	return newEntity;
}

void SceneManager::DeleteEntity(entt::entity entity) {
	if (auto* transform = Registry.try_get<Transform>(entity)) {
		//remove self from the parent(not being deleted)
		TransformSystem* transformSystem = SystemLocator::Get<TransformSystem>();

		transformSystem->DetachParent(entity);

		DFSMarkEntityDeletion(entity);
	}
	else {
		//does not have transform, just mark for deletion
		toDelete.push_back(entity);
	}
}

void SceneManager::DFSMarkEntityDeletion(entt::entity entity) {
	if (entity == entt::null) return;

	toDelete.push_back(entity);
	
	Transform* transformComponent = Registry.try_get<Transform>(entity);

	for (entt::entity child : transformComponent->GetChildren()) {
		DFSMarkEntityDeletion(child);
	}
}

void SceneManager::RemoveDeletedEntities() {
	auto transforSystem = SystemLocator::Get<TransformSystem>();

	for (auto& entity : toDelete) {
		if (Registry.valid(entity)) {
			if (auto transform = Registry.try_get<Transform>(entity)) {
				transforSystem->RemoveFromWorld(entity);
			}

			EventManager::Emit<EntityDestroyedEvent>({ entity });
			Registry.destroy(entity);
		}
		else {
			Debug::LogError("Attempting to delete entity twice");
		}
	}

	toDelete.clear();
}

void SceneManager::shutdown() {
	Registry.clear();
}