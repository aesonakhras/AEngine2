#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <vector>

#include "Graphics/IVertexShader.h"

#include "DX11VertexLayout.h"

namespace AE::Graphics {

	//forward declerations
	class DX11ShaderObject;
	struct VertexAttribute;

	class DX11VertexShader : public IVertexShader {
	public:
		DX11VertexShader(
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
			Microsoft::WRL::ComPtr<ID3D11Device> device,
			std::shared_ptr<DX11ShaderObject> shader,
			const std::vector<VertexAttribute>& attribs
		);

		bool Recreate();
		

		virtual void Bind() override final;
		virtual void UnBind() override final;

		std::shared_ptr<DX11ShaderObject> Shader;

	private:
		DX11VertexLayout m_layout;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	};
}