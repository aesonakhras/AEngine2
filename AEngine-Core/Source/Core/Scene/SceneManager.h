#pragma once
#include <memory>
#include <vector>
#include "Utils/Singleton.h"
#include "entt/entt.hpp"
#include "Core/Components/Transform.h"
#include "Core/Components/Camera.h"

namespace AE::Core {
	//for now essentially used as a tool to wrap the entt registry
	//and allow for app and core shared access, more abstraction and features to come later
	class SceneManager : public AE::Utils::Singleton<SceneManager> {
		public:
			entt::registry Registry;
			entt::entity mainCameraEntity = entt::null;

			entt::entity CreateEntity();
			void DeleteEntity(entt::entity entity);

			void RemoveDeletedEntities();

		private:
			void DFSMarkEntityDeletion(Transform* transform);

			void initialize() {};
			void shutdown();

			SceneManager() = default;
			~SceneManager() = default;

			friend class AE::Utils::Singleton<SceneManager>;

			std::vector<entt::entity> toDelete;
	};
}