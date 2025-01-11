#pragma once
#include <string>
#include "entt/entt.hpp"


namespace AE::Core {
	class SkyboxFactory {
	public:
		static entt::entity Create(
			entt::registry& registry,
			std::string cubeMapName
		);
	};
}