#pragma once
#include "entt/entt.hpp"
#include <set>
#include <DirectXMath.h>
//TODO: At some point only have name in debug config
#include <string>
#include <Core/Debug.h>

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
			entt::entity parent,
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
		
		void SetWorldRotation(DirectX::XMVECTOR rotation) {
			Rotation = rotation;

			UpdateWorldMatrix();
		}

		void SetWorldPosition(Vec3 pos) {
			Position = pos;

			UpdateWorldMatrix();
		}

		void UpdateWorldMatrix() {
			DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z);

			DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(Rotation);

			DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(Position.X, Position.Y, Position.Z);

			WorldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
		}

		void SetLocalRotation(DirectX::XMVECTOR rot) {
			Rotation = rot;
			isDirty = true;
		}

		DirectX::XMVECTOR GetLocalRotation() {
			return Rotation;
		}

		DirectX::XMVECTOR GetWorldRotation() {
			DirectX::XMMATRIX rotationMatrix = WorldMatrix;
			rotationMatrix.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
			return DirectX::XMQuaternionRotationMatrix(rotationMatrix);
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



		//const std::set<Transform*> GetChildren() {
		//	return Children;
		//}

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

		void SetParent(entt::entity parent) {			
			Parent = parent;
			isDirty = true;
		}

		const entt::entity GetParent() {
			return Parent;
		}


		void AddChild(entt::entity newChild) {
			Children.insert(newChild);
		}

		void RemoveChild(entt::entity childToRemove) {
			Children.erase(childToRemove);
		}

		const std::set<entt::entity>& GetChildren() {
			return Children;
		}

		void ClearChildren() {
			Children.clear();
		}
		std::string Name;
	private:
		entt::entity Parent;
		std::set<entt::entity> Children{};

		Vec3 Position;
		//NOTE: This is a quaternion
		DirectX::XMVECTOR Rotation;	
		Vec3 Scale;
		bool isDirty;
		
};