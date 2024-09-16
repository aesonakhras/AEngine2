#pragma once
#include <DirectXMath.h>

class Camera {
public:
	
	Camera(DirectX::FXMVECTOR position, float fov, float aspectRatio, float nearZ, float farZ);

	DirectX::XMMATRIX GetVP();

	~Camera();

private:
	DirectX::XMMATRIX m_VP;
};