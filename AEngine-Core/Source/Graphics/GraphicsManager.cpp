#include "GraphicsManager.h"
#include <iostream>
#include <comdef.h>

#include "FileManagment/FileManager.h"

#include "Core/Components/Material.h"

#ifdef D3D11_MODE
    #include "DX11IMPL/DX11GLI.h"
#endif // D3D11_MODE

using namespace AE::Graphics;

bool GraphicsManager::initialize(const DeviceCreateInfo& info) {
#ifdef D3D11_MODE
    m_GLI = std::make_unique<DX11GLI>();
#endif // D3D11_MODE
    if (m_GLI == nullptr) {
        std::cout << "GLI has not initalized, this is a fatal error";
    }
    m_GLI->Init(info);

    return true;
}


void GraphicsManager::shutdown() {
    m_GLI->ShutDown();
	return;
}

std::shared_ptr<IBuffer> GraphicsManager::CreateBuffer(const void* data, size_t count, size_t stride, BufferType bufferType) {
    return m_GLI->CreateBuffer(data, count, stride, bufferType);
}

void GraphicsManager::Draw(AE::Core::uint32 count) {
    m_GLI->Draw(count);
}

void GraphicsManager::StartFrame() {
    m_GLI->StartFrame();
}
void GraphicsManager::PresentFrame() {
    m_GLI->PresentFrame();
}

std::shared_ptr<IVertexShader> GraphicsManager::CreateVertexShader(std::string fileName, const std::vector<VertexAttribute>& attributes) {
    auto shaderData = LoadShaderRaw(fileName);

    return m_GLI->CreateVertexShader(shaderData.data(), shaderData.size(), attributes);
}

std::shared_ptr<IFragmentShader> GraphicsManager::CreateFragmentShader(std::string fileName) {
    auto shaderData = LoadShaderRaw(fileName);
    
    return m_GLI->CreateFragmentShader(shaderData.data(), shaderData.size());
}

std::vector<char> GraphicsManager::LoadShaderRaw(std::string fileName) {
    //Load File
    AE::System::FileManager& fileManager = AE::System::FileManager::GetInstance();

    auto fileHandle = fileManager.GetFile(fileName, AE::System::FileOperation::Read);

    return fileHandle->ReadAll();
}

std::shared_ptr<Material> GraphicsManager::CreateMaterial(
    std::string shaderName,
    const std::vector<VertexAttribute>& attributes,
    const void* initalData,
    size_t intialDataSize,
    std::vector<UniformDescription> uniformDescription
) 
{
    auto vShader = CreateVertexShader(shaderName, attributes);
    auto pShader = CreateFragmentShader(shaderName);
    auto ubo = CreateBuffer(initalData, intialDataSize, 0, BufferType::Uniform);
    

    return std::make_shared<Material>(vShader, pShader, ubo, uniformDescription);
}


std::shared_ptr<Texture> GraphicsManager::CreateTexture(const AE::Graphics::TextureCreateInfo& info) {
    auto textureResource = m_GLI->CreateTextureResource(info);
    
    auto shaderResourceView = m_GLI->CreateShaderResourceView(textureResource);
    

    return std::make_shared<Texture>(shaderResourceView, textureResource);
}

std::shared_ptr<ISampler> GraphicsManager::CreateSampler() {
    auto sampler = m_GLI->CreateSampler();

    return sampler;
}