#pragma once
#include <memory>
#include <entt/entt.hpp>

#include "Core/Types.h"

namespace AE::App {
	struct WindMill {
		float rotationSpeed;

		entt::entity rotor1;
		entt::entity rotor2;
		entt::entity rotor3;
		entt::entity rotor4;

		entt::entity pin;
		float TimeSinceShot;
		int health = 4;

		
	};
}