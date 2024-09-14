#include "GraphicsManager.h"
#include <iostream>
#include <comdef.h>

#include "FileManagment/FileManager.h"

#ifdef D3D11_MODE
    #include "D3D11GLI.h"
#endif // D3D11_MODE

using namespace AE::Graphics;

bool GraphicsManager::Initialize(AECore::DeviceCreateInfo info) {
#ifdef D3D11_MODE
    m_GLI = std::make_unique<D3D11GLI>();
#endif // D3D11_MODE
    if (m_GLI == nullptr) {
        std::cout << "GLI has not initalized, this is a fatal error";
    }
    m_GLI->Init(info);

    return true;
}


void GraphicsManager::ShutDown() {
    m_GLI->ShutDown();
	return;
}

Microsoft::WRL::ComPtr <ID3D11Device> GraphicsManager::GetDevice() {
    return m_GLI->GetDevice();
}

Microsoft::WRL::ComPtr <ID3D11DeviceContext> GraphicsManager::GetDeviceContext() {
    return m_GLI->GetDeviceContext();
}

void GraphicsManager::DrawMesh(const StaticMesh& mesh, DirectX::XMMATRIX VP) {
    //get the mvp from the mesh, potentially just get the m, and then handle the VP from here l8r

    //update the mvp matrix in the vertexShader
    //mesh.m_vertexShader->SetMVP(mesh.m_modelMatrix * VP);

    mesh.Bind();
    
    m_GLI->GetDeviceContext()->DrawIndexed(mesh.GetCount(), 0, 0);
}

std::shared_ptr<IBuffer> GraphicsManager::CreateBuffer(const void* data, size_t count, size_t stride, BufferType bufferType) {
    return m_GLI->CreateBuffer(data, count, stride, bufferType);
}

void GraphicsManager::DrawFrame(std::vector<StaticMesh*> meshes, DirectX::XMMATRIX VP) {
    m_GLI->Clear();

    // select which vertex buffer to display
    for (auto mesh : meshes) {
        DrawMesh(*mesh, VP);
    }

    m_GLI->Swap();
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
    AE::Core::System::FileManager& fileManager = AE::Core::System::FileManager::GetInstance();

    auto fileHandle = fileManager.GetFile(fileName, AE::Core::System::FileOperation::Read);

    return fileHandle->ReadAll();
}

std::shared_ptr<AE::Graphics::Material> CreateMaterial(std::string shaderName) {
    //IGlI->CreateShader(Vertex)
    //IGlie->CreateHsader(Fragment)
    //IGlie->Layout
    //IGli->Cra
    return nullptr;
}


std::shared_ptr<Texture> GraphicsManager::CreateTexture(const AE::Graphics::TextureCreateInfo& info) {
    auto textureResource = m_GLI->CreateTextureResource(info);
    
    auto shaderResourceView = m_GLI->CreateShaderResourceView(textureResource);
    auto sampler = m_GLI->CreateSampler();

    return std::make_shared<Texture>(shaderResourceView, sampler, textureResource);
}