#pragma once
#include "Utils/Singleton.h"
#include "entt/entt.hpp"

namespace AE::Core {
	//for now essentially used as a tool to wrap the entt registry
	//and allow for app and core shared access, more abstraction and features to come later
	class SceneManager : public AE::Utils::Singleton<SceneManager> {
		public:
			entt::registry Registry;
		private:
			void initialize() {};
			void shutdown() {};

			SceneManager() = default;
			~SceneManager() = default;

			friend class AE::Utils::Singleton<SceneManager>;
	};
}