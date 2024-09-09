#pragma once

//TODO: until I implement openGL, Vulkam, or DX12 directX11 will live here
//not the cleanest solution but I literally spent a day trying to design
//a system just guessing what I will need.  Also doing this will avoid a Virtual

//TODO: This needs to be replaced with Legit error code,
//but for not lets just use IOStream until I implement that code
#include <iostream>
#include <d3d11.h>
#include "DeviceCreateInfo.h"
#include "StaticMesh.h"
#include <vector>
#include <string>
#include "AEngineVertexTypes.h"
#include "../Material.h"
#include "Texture.h"

#include "IGLI.h"
#include "Graphics/TextureCreateInfo.h"

//Forward declares to reduce compile time?
class IFragmentShader;

namespace AECore {
	class GraphicsManager {

	public:
		GraphicsManager() {/* intentianlly left blank Init will initialize, this is to allow start up to be done in programmer order*/ };
		~GraphicsManager() {/*same as above*/ }

		bool Initialize(AECore::DeviceCreateInfo info);
		void DrawFrame(std::vector<StaticMesh*> meshes, DirectX::XMMATRIX VP);
		void ShutDown();

		Microsoft::WRL::ComPtr <ID3D11Device> GetDevice();
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> GetDeviceContext();

		//CreateBuffer
		std::shared_ptr<IBuffer> CreateBuffer(const void* data, size_t count, size_t stride, AEngine::Graphics::BufferType bufferType);

		std::shared_ptr<IFragmentShader> CreateFragmentShader(std::string fileName);

		std::shared_ptr<AEngine::Graphics::Material> CreateMaterial(std::string shaderName);


		//Pass in the data to the texture, call it make texture
		std::shared_ptr<Texture> CreateTexture(const AE::Core::Graphics::TextureCreateInfo& info);

	private:
		void DrawMesh(const StaticMesh& mesh, DirectX::XMMATRIX VP);

		//TODO: Do not like this on the graphics manager
		std::vector<char> LoadShaderRaw(std::string fileName);

		std::unique_ptr<IGLI> m_GLI = nullptr;

		UINT m_stride = sizeof(AEngineVertexTypes::VERTEX);
		UINT m_offset = 0;
	};
}