#pragma once
#include "entt/entt.hpp"
#include <set>
#include <DirectXMath.h>
//TODO: At some point only have name in debug config
#include <string>

#include "Math/Vec3.h"


//TODO: implement matrix to get rid of the directX internal dependency
struct Transform {
	public:
		DirectX::XMMATRIX WorldMatrix;

		Transform() = delete;

		Transform (
			entt::entity entity,
			Vec3 pos,
			DirectX::XMVECTOR rot,
			Vec3 scale,
			Transform* parent,
			std::string name
		) : 
			Position { pos }, 
			Rotation{ rot },
			Scale{ scale },
			isDirty{ true },
			Parent { parent },
			Name{ name },
			WorldMatrix{ DirectX::XMMatrixIdentity() },
			Entity(entity)
		{
			if (Parent != nullptr) {
				Parent->AddChild(this);
			}	
		}

		~Transform() {
			if (Parent) {
				Parent->RemoveChild(this);
			}
			//clear children
			for (auto* child : Children) {
				child->Parent = nullptr;
			}
		}

		void SetLocalPosition(Vec3 pos) {
			Position = pos;
			isDirty = true;
		}

		Vec3 GetLocalPosition() {
			return Position;
		}

		//TODO: Some sort of logging ability

		Vec3 GetWorldPosition() {
			return Vec3(WorldMatrix.r[3].m128_f32[0],
				WorldMatrix.r[3].m128_f32[1],
				WorldMatrix.r[3].m128_f32[2]);
		}

		void SetLocalRotation(DirectX::XMVECTOR rot) {
			Rotation = rot;
			isDirty = true;
		}

		DirectX::XMVECTOR GetLocalRotation() {
			return Rotation;
		}

		void SetScale(Vec3 scale) {
			Scale = scale;
			isDirty = true;
		}

		Vec3 GetScale() {
			return Scale;
		}

		//DO not use these in the application layer, you will break everything
		bool GetDirty() {
			return isDirty;
		}

		//DO not use these in the application layer, you will break everything
		void SetDirty() {
			isDirty = true;
		}

		//DO not use these in the application layer, you will break everything
		void ClearDirty() {
			isDirty = false;
		}

		DirectX::XMMATRIX GetLocalMatrix() {
			DirectX::XMVECTOR dxPos = { Position.X, Position.Y, Position.Z };
			DirectX::XMVECTOR dxScale = { Scale.X, Scale.Y, Scale.Z };

			DirectX::XMVECTOR normalizedRotation = DirectX::XMQuaternionNormalize(Rotation);

			return DirectX::XMMatrixScalingFromVector(dxScale) *
				DirectX::XMMatrixRotationQuaternion(normalizedRotation) *
				DirectX::XMMatrixTranslationFromVector(dxPos);
		}

		void SetParent(Transform* parent) {
			if (Parent != nullptr) {
				Parent->AddChild(this);
			}

			isDirty = true;
		}

		void DetachParent() {
			if (Parent != nullptr) {
				Parent->RemoveChild(this);
				Parent = nullptr;
			}

			isDirty = true;
		}

		const Transform* GetParent() {
			return Parent;
		}

		const std::set<Transform*> GetChildren() {
			return Children;
		}

		const Vec3 GetForwardDir() {

			DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

			DirectX::XMVECTOR worldForward = DirectX::XMVector3TransformNormal(forward, WorldMatrix);

			worldForward = DirectX::XMVector3Normalize(worldForward);

			DirectX::XMFLOAT3 forwardVec3;
			DirectX::XMStoreFloat3(&forwardVec3, worldForward);

			return { forwardVec3.x, forwardVec3.y, forwardVec3.z };
		}

		entt::entity Entity;
		Vec3 worldPosition;

	private:
		void AddChild(Transform* newChild) {
			Children.insert(newChild);
		}

		void RemoveChild(Transform* childToRemove) {
			Children.erase(childToRemove);
		}

		Vec3 Position;
		//NOTE: This is a quaternion
		DirectX::XMVECTOR Rotation;	
		Vec3 Scale;
		bool isDirty;
		Transform* Parent;
		std::set<Transform*> Children{};
		std::string Name;
};