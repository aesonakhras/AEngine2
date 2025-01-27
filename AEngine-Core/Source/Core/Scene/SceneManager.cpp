#include "SceneManager.h"


using namespace AE::Core;

entt::entity SceneManager::CreateEntity() {
	entt::entity newEntity = Registry.create();

	//TODO_E: Code to handle entity creation failure

	return newEntity;
}

void SceneManager::DeleteEntity(entt::entity entity) {
	if (auto transform = Registry.try_get<Transform>(entity)) {
		//remove self from the parent(not being deleted)
		transform->DetachParent();

		DFSMarkEntityDeletion(transform);
	}
	else {
		//does not have transform, just mark for deletion
		toDelete.push_back(entity);
	}
}

void SceneManager::DFSMarkEntityDeletion(Transform* transform) {
	toDelete.push_back(transform->Entity);
	
	for (Transform* child : transform->GetChildren()) {
		DFSMarkEntityDeletion(child);
	}
}

void SceneManager::RemoveDeletedEntities() {
	for (auto entity : toDelete) {
		Registry.destroy(entity);
	}

	toDelete.clear();
}

void SceneManager::shutdown() {
	Registry.clear();
}