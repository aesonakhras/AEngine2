#include "GraphicsManager.h"
#include <iostream>
#include <comdef.h>

#ifdef D3D11_MODE
    #include "D3D11GLI.h"
#endif // D3D11_MODE


bool AECore::GraphicsManager::Initialize(AECore::DeviceCreateInfo info) {
#ifdef D3D11_MODE
    m_GLI = std::make_unique<D3D11GLI>();
#endif // D3D11_MODE
    if (m_GLI == nullptr) {
        std::cout << "GLI has not initalized, this is a fatal error";
    }
    m_GLI->Init(info);

    return true;
}


void AECore::GraphicsManager::ShutDown() {
    m_GLI->ShutDown();
	return;
}

ID3D11Device* AECore::GraphicsManager::GetDevice() {
    return m_GLI->GetDevice();
}

ID3D11DeviceContext* AECore::GraphicsManager::GetDeviceContext() {
    return m_GLI->GetDeviceContext();
}

void AECore::GraphicsManager::DrawMesh(const StaticMesh& mesh, DirectX::XMMATRIX VP) {
    //get the mvp from the mesh, potentially just get the m, and then handle the VP from here l8r

    //update the mvp matrix in the vertexShader
    mesh.m_vertexShader->SetMVP(mesh.m_modelMatrix * VP);

    mesh.m_vertexBuffer->Bind(m_stride, 0);

    m_GLI->BindBuffer(mesh.m_indexBuffer);
    //mesh.m_indexBuffer->Bind(DXGI_FORMAT_R32_UINT, 0);
    mesh.m_vertexShader->Bind();
    mesh.m_fragmentShader->Bind();

    
    m_GLI->GetDeviceContext()->DrawIndexed(mesh.m_indexBuffer->IndicesCount, 0, 0);
}

void AECore::GraphicsManager::DrawFrame(std::vector<StaticMesh*> meshes, DirectX::XMMATRIX VP) {
    m_GLI->Clear();

    // select which vertex buffer to display


    for (auto mesh : meshes) {
        DrawMesh(*mesh, VP);
    }

    m_GLI->Swap();
}