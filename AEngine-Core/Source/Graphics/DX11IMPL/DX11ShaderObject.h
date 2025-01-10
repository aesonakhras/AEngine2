#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <string>

#include "Core/Debug.h"

#include "FileManagment/FileManager.h"

namespace AE::Graphics {
	class DX11ShaderObject {
	public:
		DX11ShaderObject(
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
			Microsoft::WRL::ComPtr<ID3D11Device> device,
			const void* data,
			size_t dataSize,
			std::string entryPoint,
			std::string shaderTarget);

		Microsoft::WRL::ComPtr <ID3D10Blob> GetShaderBlob();

		bool Compile(
			const void* data,
			size_t dataSize,
			std::string entryPoint,
			std::string shaderTarget
		);

	private:
		Microsoft::WRL::ComPtr <ID3D10Blob> m_shaderBlob = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device = nullptr;
	};
}