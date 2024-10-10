#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "Texture.h"

#include "Core/Common.h"

#include "IGLI.h"
#include "TextureCreateInfo.h"
#include "Utils/Singleton.h"

namespace AE::Graphics {

	//Forward declares to reduce compile time?
	class IFragmentShader;
	class IVertexShader;
	struct VertexAttribute;
	class Material;
	struct DeviceCreateInfo;

	class GraphicsManager : public AE::Utils::Singleton<GraphicsManager> {

	public:
		GraphicsManager() {/* intentianlly left blank Init will initialize, this is to allow start up to be done in programmer order*/ };
		~GraphicsManager() {/*same as above*/ }
		
		void Draw(AE::Core::uint32 count);

		void StartFrame();
		void PresentFrame();

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

	protected:
		bool initialize(const DeviceCreateInfo& info);
		void shutdown();

	private:

		//TODO: Do not like this on the graphics manager
		std::vector<char> LoadShaderRaw(std::string fileName);

		std::unique_ptr<IGLI> m_GLI = nullptr;

		unsigned int m_offset = 0;

		friend class AE::Utils::Singleton<GraphicsManager>;
	};
}