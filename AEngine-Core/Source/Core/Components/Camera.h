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
		bool IsOrthographic;
		float32 OrthographicSize;

		Camera(
			float32 fov,
			float32 aspectRatio,
			float32 nearZ,
			float32 farZ,
			bool isOrthographic,
			float32 orthographicSize
		) : Fov(fov),
			AspectRatio(aspectRatio),
			NearZ(nearZ),
			FarZ(farZ),
			IsOrthographic(isOrthographic),
			OrthographicSize(orthographicSize)
		{
			ProjectionMatrix = UpdateProjectionMatrix();
		}

		DirectX::XMMATRIX UpdateProjectionMatrix() {
			if (IsOrthographic) {
				return DirectX::XMMatrixOrthographicOffCenterLH(
					-OrthographicSize/2.0f, OrthographicSize / 2.0f,
					-OrthographicSize/2.0f, OrthographicSize / 2.0f,
					NearZ,
					FarZ
				);
			}
			else {
				return DirectX::XMMatrixPerspectiveFovLH(
					Fov,
					AspectRatio,
					NearZ,
					FarZ
				);
			}
		}
		
	};
}