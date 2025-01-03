#include "Core/Systems/TransformSystem.h"
#include "Core/Scene/SceneManager.h"

void PrintMatrix(const DirectX::XMMATRIX& matrix, std::string name) {
	// Convert the SIMD matrix to a 4x4 float matrix for easier access
	DirectX::XMFLOAT4X4 matValues;
	DirectX::XMStoreFloat4x4(&matValues, matrix);

	// Print the matrix row by row
	std::cout << "Matrix: " << name << std::endl;
	for (int row = 0; row < 4; ++row) {
		std::cout << matValues.m[row][0] << " " << matValues.m[row][1] << " "
			<< matValues.m[row][2] << " " << matValues.m[row][3] << std::endl;
	}
	std::cout << std::endl;
}

using namespace AE::Core;

TransformSystem::TransformSystem()
{

}

void TransformSystem::SetScene(entt::registry& _registry) {
	registry = &_registry;
}

void TransformSystem::SetLocalPosition(entt::entity entity, Vec3 newPos) {
	transformCommandbuffer.Submit([this, entity, newPos]() {
		auto& transform = registry->get<Transform>(entity);
		transform.SetDirty();

		transform.SetLocalPosition(newPos);
	});
}

void TransformSystem::SetLocalRotation(entt::entity entity, DirectX::XMVECTOR newRot) {
	transformCommandbuffer.Submit([this, entity, newRot]() {
		auto& transform = registry->get<Transform>(entity);
		transform.SetDirty();

		transform.SetLocalRotation(newRot);
	});
}

DirectX::XMVECTOR TransformSystem::GetLocalRotation(entt::entity entity){
	auto& transform = registry->get<Transform>(entity);
	return transform.GetLocalRotation();
}


void TransformSystem::Update() {
	//apply the transforms
	transformCommandbuffer.Execute();

	//now recalculate the world matricies if necessary

	auto transformView = registry->view<Transform>();

	for (auto entity : transformView) {
		auto& transform = transformView.get<Transform>(entity);

		//dfs with root nodes only
		if (transform.GetParent() == nullptr) {
			transform.WorldMatrix = transform.GetLocalMatrix();

			updateWorldMatrix(&transform, transform.WorldMatrix, transform.GetDirty());

			transform.ClearDirty();
		}
	}
}

void TransformSystem::updateWorldMatrix(
	Transform* parentTransform,
	DirectX::XMMATRIX parentWorldMatrix,
	bool parentDirtyStatus
) {
	if (parentTransform == nullptr) return;

	

	for (Transform* childTransform : parentTransform->GetChildren()) {

		bool shouldUpdate = childTransform->GetDirty() || parentDirtyStatus;
		
		if (shouldUpdate) {
			childTransform->WorldMatrix = childTransform->GetLocalMatrix() * parentWorldMatrix;
			childTransform->ClearDirty();
		}

		updateWorldMatrix(childTransform, childTransform->WorldMatrix, shouldUpdate);
	}
}

Vec3 TransformSystem::GetLocalPosition(entt::entity transform) {
	auto& parentTransform = registry->get<Transform>(transform);
	
	return parentTransform.GetLocalPosition();
}