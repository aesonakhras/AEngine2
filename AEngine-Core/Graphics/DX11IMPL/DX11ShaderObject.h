#pragma once
#include "../IShaderObject.h"
#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include "d3dcompiler.h"
#include "../Debug.h"
#include "../FileManagment/FileManager.h"

class DX11ShaderObject : public IShaderObject {
public:
	DX11ShaderObject(
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		const void* data,
		size_t dataSize,
		std::string entryPoint,
		std::string shaderTarget);

	virtual void* GetRawObject() final override;

private:
	Microsoft::WRL::ComPtr <ID3D10Blob> m_shaderBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> m_device = nullptr;
};