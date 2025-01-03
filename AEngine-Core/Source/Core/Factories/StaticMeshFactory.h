#pragma once
#include <entt/entt.hpp>

#include <string>
#include <memory>
#include <DirectXMath.h>

#include "Core/Components/Transform.h"
#include "Core/Components/Material.h"
#include "Graphics/Mesh.h"
#include "Math/Vec3.h"


namespace AE::Core {	
	class StaticMeshFactory {
		public:
			static entt::entity Create(
				entt::registry& registry,
				AE::Graphics::Mesh meshName,
				AE::Graphics::Material& material,
				Vec3 pos,
				DirectX::XMVECTOR rot,
				Vec3 scale,
				Transform* parent,
				std::string name
			);
	};
}