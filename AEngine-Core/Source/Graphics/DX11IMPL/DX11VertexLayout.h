#include <vector>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>

#include "Graphics/IVertexLayout.h"

namespace AE::Graphics {

	//forward declares
	struct VertexAttribute;
	class IVertexDescription;

	class DX11VertexLayout : public IVertexLayout {

		public:
			DX11VertexLayout(
				Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
				Microsoft::WRL::ComPtr<ID3D11Device> device
			);


			void Build(const std::vector<VertexAttribute>& vertexAttributes, Microsoft::WRL::ComPtr<ID3D10Blob> shaderBlob);
			void Rebuild(Microsoft::WRL::ComPtr<ID3D10Blob> shaderBlob);
			virtual void Bind() override final;
			virtual void UnBind() override final;

		private:
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
			Microsoft::WRL::ComPtr<ID3D11Device> m_device;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
			std::vector<D3D11_INPUT_ELEMENT_DESC> m_elementDescs{ };
	};
}