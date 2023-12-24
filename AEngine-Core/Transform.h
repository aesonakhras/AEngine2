#pragma once
#include <DirectXMath.h>

struct Transform {
	//TODO: Need to abstract this out before too long
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT3 Rotation;	//euler

	Transform(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotation) {
		Position = position;
		Scale = scale;
		Rotation = rotation;
	}
};