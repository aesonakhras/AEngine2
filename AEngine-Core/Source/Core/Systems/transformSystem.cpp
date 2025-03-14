#include <iostream>

#include "Core/Systems/TransformSystem.h"
#include "Core/Scene/SceneManager.h"

#include "Core/Components/RigidBody.h"

using namespace AE::Core;
using namespace AE::Physics;

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



void TransformSystem::AddToWorld(entt::entity entity, entt::entity parent)
{
	if (parent != entt::null) {
		AttachParent(entity, parent);
	}
}

void TransformSystem::RemoveFromWorld(entt::entity entity) {
	Transform* childTransform = registry->try_get<Transform>(entity);

	if (!childTransform) return;

	DetachParent(entity);
}

void TransformSystem::AttachParent(entt::entity child, entt::entity parent) {
	Transform* childTransform = registry->try_get<Transform>(child);
	Transform* parentTransform = registry->try_get<Transform>(parent);

	if (!childTransform || !parentTransform) return;

	if (childTransform->GetParent() != entt::null) {
		DetachParent(child);
	}

	childTransform->SetParent(parent);
	parentTransform->AddChild(child);
}

void TransformSystem::DetachParent(entt::entity child) {
	Transform* childTransform = registry->try_get<Transform>(child);

	if (!childTransform) return;

	entt::entity parentEntity = childTransform->GetParent();
	if (parentEntity == entt::null) return;

	Transform* parentTransform = registry->try_get<Transform>(parentEntity);

	if (parentTransform) {
		parentTransform->RemoveChild(child);
	}

	childTransform->SetParent(entt::null);
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
		if (transform.GetParent() == entt::null) {
			if (transform.GetDirty()) {
				transform.WorldMatrix = transform.GetLocalMatrix();
				registry->emplace<TransformUpdatedTag>(transform.Entity);
			}

			updateWorldMatrix(&transform, transform.WorldMatrix, transform.GetDirty());

			transform.ClearDirty();
		}
	}
}

void TransformSystem::UpdateKinematicTransforms() {

}

void TransformSystem::updateWorldMatrix(
	Transform* parentTransform,
	DirectX::XMMATRIX parentWorldMatrix,
	bool parentDirtyStatus
) {
	if (parentTransform == nullptr) return;

	if (parentTransform->GetChildren().empty()) return;

	for (entt::entity childTransformEntity : parentTransform->GetChildren()) {
		Transform* childTransformComponent = registry->try_get<Transform>(childTransformEntity);


		bool shouldUpdate = childTransformComponent->GetDirty() || parentDirtyStatus;
		
		if (shouldUpdate) {
			childTransformComponent->WorldMatrix = childTransformComponent->GetLocalMatrix() * parentWorldMatrix;


			registry->emplace<TransformUpdatedTag>(childTransformComponent->Entity);

			childTransformComponent->ClearDirty();
		}

		updateWorldMatrix(childTransformComponent, childTransformComponent->WorldMatrix, shouldUpdate);
	}
}

void TransformSystem::ClearUpdated() {
	registry->clear<TransformUpdatedTag>();
}

Vec3 TransformSystem::GetLocalPosition(entt::entity transform) {
	auto& parentTransform = registry->get<Transform>(transform);
	
	return parentTransform.GetLocalPosition();
}