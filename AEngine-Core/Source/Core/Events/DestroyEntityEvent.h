#pragma once

#include <entt/entt.hpp>

struct EntityDestroyedEvent 
{
	entt::entity entity;

	EntityDestroyedEvent(entt::entity e) : entity(e) {}
};