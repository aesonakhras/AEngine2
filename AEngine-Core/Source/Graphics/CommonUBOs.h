#pragma once
#include <DirectXMath.h>

namespace AE::Graphics {
	struct StandardUniformBuffer
	{
		DirectX::XMMATRIX mWorldViewProj;
		DirectX::XMMATRIX modelView;
		DirectX::XMVECTOR viewDir;
		DirectX::XMVECTOR dirLightDir;
	};
}