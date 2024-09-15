#pragma once
#include <vector>
#include <string>

#include <iostream>


#include "../Graphics/StaticMesh.h"
#include "../Graphics/Texture.h"
#include "IGLI.h"
#include "TextureCreateInfo.h"

namespace AE::Graphics {

	//Forward declares to reduce compile time?
	class IFragmentShader;
	class IVertexShader;
	struct VertexAttribute;
	class Material;
	struct DeviceCreateInfo;

	class GraphicsManager {

	public:
		GraphicsManager() {/* intentianlly left blank Init will initialize, this is to allow start up to be done in programmer order*/ };
		~GraphicsManager() {/*same as above*/ }

		bool Initialize(const DeviceCreateInfo& info);
		void DrawFrame(std::vector<StaticMesh*> meshes, DirectX::XMMATRIX VP);
		void ShutDown();

		//CreateBuffer
		std::shared_ptr<IBuffer> CreateBuffer(const void* data, size_t count, size_t stride, BufferType bufferType);

		std::shared_ptr<IVertexShader> CreateVertexShader(std::string fileName, const std::vector<VertexAttribute>& attributes);

		std::shared_ptr<IFragmentShader> CreateFragmentShader(std::string fileName);

		std::shared_ptr<Material> CreateMaterial(
			std::string shaderName,
			const std::vector<VertexAttribute>& attributes,
			const void* initalData,
			size_t intialDataSize
		);

		//Pass in the data to the texture, call it make texture
		std::shared_ptr<Texture> CreateTexture(const AE::Graphics::TextureCreateInfo& info);

		std::shared_ptr<ISampler> CreateSampler();

	private:
		void DrawMesh(StaticMesh& mesh, DirectX::XMMATRIX VP);

		//TODO: Do not like this on the graphics manager
		std::vector<char> LoadShaderRaw(std::string fileName);

		std::unique_ptr<IGLI> m_GLI = nullptr;

		unsigned int m_offset = 0;
	};
}