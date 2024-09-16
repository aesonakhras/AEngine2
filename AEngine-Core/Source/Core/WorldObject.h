#pragma once
#include <DirectXMath.h>
#include "Core/Transform.h"

//for now this simply will contain a position
namespace AE::Core {
	struct WorldObject {
		WorldObject() : ModelMatrix(DirectX::XMMatrixIdentity()), hasBeenModified(false) { }

		void SetPosition(DirectX::XMVECTOR position) {
			transform.Position = position;
			hasBeenModified = true;
		}

		void SetRotation(DirectX::XMVECTOR rotation) {
			transform.Rotation = rotation;
			hasBeenModified = true;
		}

		void SetScale(DirectX::XMVECTOR scale) {
			transform.Scale = scale;
			hasBeenModified = true;
		}

		DirectX::XMVECTOR GetPosition() {
			return transform.Position;
		}

		DirectX::XMVECTOR GetRotation() {
			return transform.Rotation;
		}

		DirectX::XMVECTOR GetScale() {
			return transform.Scale;
		}

		DirectX::XMMATRIX GetMatrix() {
			if (hasBeenModified) {
				UpdateMatrix();
				hasBeenModified = false;
			}

			return ModelMatrix;
		}

	private:
		void UpdateMatrix() {
			ModelMatrix = DirectX::XMMatrixScalingFromVector(transform.Scale) *
						  DirectX::XMMatrixRotationRollPitchYawFromVector(transform.Rotation)*
						  DirectX::XMMatrixTranslationFromVector(transform.Position);
		}

		Transform transform;
		DirectX::XMMATRIX ModelMatrix;
		bool hasBeenModified;
	};
}