#include "Camera.h"

Camera::Camera(DirectX::FXMVECTOR position, float fov, float aspectRatio, float nearZ, float farZ) {
	DirectX::XMMATRIX ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);

	 DirectX::FXMVECTOR focusPosition = {0.0f, 0.0f, 0.0f, 0.0f};

	 DirectX::XMMATRIX ViewMatrix = DirectX::XMMatrixLookAtLH(position, focusPosition, {0.0f, 1.0f, 0.0f});

	 m_VP = ViewMatrix * ProjectionMatrix;
}


DirectX::XMMATRIX Camera::GetVP() {
	return m_VP;
}

Camera::~Camera() {

}
