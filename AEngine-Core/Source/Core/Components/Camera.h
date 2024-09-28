#pragma once
#include <DirectXMath.h>

#include "Core/Common.h"

namespace AE::Core {
	struct Camera {
		float32 Fov;
		float32 AspectRatio;
		float32 NearZ;
		float32 FarZ;
		DirectX::XMMATRIX ProjectionMatrix;

		Camera(
			float32 fov,
			float32 aspectRatio,
			float32 nearZ,
			float32 farZ
		) : Fov(fov),
			AspectRatio(aspectRatio),
			NearZ(nearZ),
			FarZ(farZ)
		{
			ProjectionMatrix = UpdateProjectionMatrix();
		}

		DirectX::XMMATRIX UpdateProjectionMatrix() {
			return DirectX::XMMatrixPerspectiveFovLH(Fov, AspectRatio, NearZ, FarZ);
		}
		
	};
}