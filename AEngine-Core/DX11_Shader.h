#pragma once
#include "../IShader.h"
#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include "d3dcompiler.h"
#include "Debug.h"
#include "../FileManagment/FileManager.h"

class DX11_Shader : public IShader {
public:
	DX11_Shader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
		Microsoft::WRL::ComPtr <ID3D11Device> device, std::string pFileName, AEngine::Graphics::ShaderType shaderType);
	
	virtual void Bind() const final override;
	virtual void UnBind() const final override;
private:

	//Note 
	union U_Shader
	{
		Microsoft::WRL::ComPtr <ID3D11VertexShader> VS = nullptr;
		Microsoft::WRL::ComPtr <ID3D11PixelShader> FS;

		U_Shader() { }

		~U_Shader() {
			//NOTE: you need to manually call the destructor on the VS or FS
		}

	};

	U_Shader m_shader;
	AEngine::Graphics::ShaderType m_shaderType;

	Microsoft::WRL::ComPtr <ID3D10Blob> m_shaderBlob = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext = nullptr;
};