#pragma once
#include <memory>	//for the smart ptr
#include <d3d11.h>
#include <wrl/client.h>

#include  "Graphics/IFragmentShader.h"

namespace AE::Graphics {
	//forward declerations
	class DX11ShaderObject;

	class DX11FragmentShader : public IFragmentShader {
	public:

		DX11FragmentShader(
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
			Microsoft::WRL::ComPtr<ID3D11Device> device,
			std::shared_ptr<DX11ShaderObject> shader
		);

		bool Recreate();

		virtual void Bind() final override;
		virtual void UnBind() final override;

		std::shared_ptr<DX11ShaderObject> Shader;
	private:
		
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	};
}