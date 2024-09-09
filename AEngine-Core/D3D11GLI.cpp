#include <iostream>

#include  "D3D11GLI.h"

#include <comdef.h>

#include "DX11_Buffer.h"
#include "Graphics/DX11IMPL/DX11ShaderObject.h"
#include "Graphics/DX11IMPL/DX11Sampler.h"
#include "Graphics/DX11IMPL/DX11ShaderResourceView.h"
#include "Graphics/DX11IMPL/DX11TextureResource.h"
#include "Graphics/DX11IMPL/DX11FragmentShader.h"

//TODO: you must remove this
using namespace AE::Core::Graphics;

void AECore::D3D11GLI::PrintHResult(HRESULT result) {
    _com_error err(result);
    LPCTSTR errMsg = err.ErrorMessage();

    std::wcout << errMsg << std::endl;
}

Microsoft::WRL::ComPtr <ID3D11Device> AECore::D3D11GLI::GetDevice() {
    return m_device;
}

Microsoft::WRL::ComPtr <ID3D11DeviceContext> AECore::D3D11GLI::GetDeviceContext() {
    return m_deviceContext;
}

void AECore::D3D11GLI::Init(AECore::DeviceCreateInfo info) {
    DXGI_SWAP_CHAIN_DESC desc;

    //TODO: Is this very c++ like
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    desc.BufferCount = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.Width = info.ScreenWidth;                    // set the back buffer width
    desc.BufferDesc.Height = info.ScreenHeight;                  // set the back buffer height
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = info.Window.GetWindowHandle<HWND>();;
    desc.SampleDesc.Count = 4;
    desc.Windowed = TRUE;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    //TODO: This is generating a warning, go ahead and fix this
    D3DCreateCall(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
        NULL, NULL, D3D11_SDK_VERSION, &desc, &m_swapChain, &m_device, NULL, &m_deviceContext), "DX11 has failed to start\n");

    //Depth
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));

    descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    descDSV.Texture2D.MipSlice = 0;

    //get the texure
    m_depthTexture = CreateTextureD3D(nullptr, info.ScreenHeight, info.ScreenWidth, 1,
        DXGI_FORMAT_D32_FLOAT_S8X24_UINT, 4, D3D11_BIND_DEPTH_STENCIL);

    // Create the depth stencil view

    D3DCreateCall(m_device->CreateDepthStencilView(m_depthTexture.Get(), // Depth stencil texture
        &descDSV, // Depth stencil desc
        &m_pDSV), "Error creating depth stencil view.");  // [out] Depth stencil view

    //now lets set up the render target
    ID3D11Texture2D* pBackBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    D3DCreateCall(m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer), "failed to Create render target.");
    pBackBuffer->Release();

    //screenQuadTexture = std::make_shared<Texture>(device, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 4, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
    //device->CreateRenderTargetView(screenQuadTexture->GetTexture(), NULL, &screenQuadRenderTargetView);


    m_deviceContext->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_pDSV.Get());

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = info.ScreenWidth;
    viewport.Height = info.ScreenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_deviceContext->RSSetViewports(1, &viewport);

    //set up the raster state
    D3D11_RASTERIZER_DESC  rasterDesc;

    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = FALSE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0;
    rasterDesc.SlopeScaledDepthBias = 0;
    rasterDesc.DepthClipEnable = FALSE;
    rasterDesc.ScissorEnable = FALSE;
    rasterDesc.MultisampleEnable = TRUE;
    rasterDesc.AntialiasedLineEnable = TRUE;

    ID3D11RasterizerState* rasterState;
    D3DCreateCall(m_device->CreateRasterizerState(&rasterDesc, &rasterState), "Unable to create Raster State");

    m_deviceContext->RSSetState(rasterState);

    setupDepthStencilState();

    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    std::cout << "Graphics initalized" << std::endl;
}

void AECore::D3D11GLI::setupDepthStencilState() {
    D3D11_DEPTH_STENCIL_DESC dsDesc;

    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // Stencil test parameters
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    ID3D11DepthStencilState* pDSState;
    D3DCreateCall(m_device->CreateDepthStencilState(&dsDesc, &pDSState), "Unable to create Depth Stencil state");
    m_deviceContext->OMSetDepthStencilState(NULL, 0);
}

ID3D11Texture2D* AECore::D3D11GLI::CreateTextureD3D(void* data, unsigned int height, unsigned int width, unsigned int miplevel,
    DXGI_FORMAT format, unsigned int sampleCount, unsigned int BindFlags) {
    ID3D11Texture2D* texture = nullptr;

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = miplevel;
    desc.ArraySize = 1;
    desc.Format = format;
    desc.SampleDesc.Count = sampleCount;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = BindFlags; //change
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    std::string errorMsg("Unable to create texture.");

    if (data == nullptr) {
        //for depth/stencil buffer
        D3DCreateCall(m_device->CreateTexture2D(&desc, NULL, &texture), errorMsg);
    }
    else {
        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = data;
        initData.SysMemPitch = width * 4;
        initData.SysMemSlicePitch = 0;

        D3DCreateCall(m_device->CreateTexture2D(&desc, &initData, &texture), errorMsg);
    }

    return texture;
}

void AECore::D3D11GLI::Clear() {
    m_deviceContext->ClearRenderTargetView(m_backBuffer.Get(), RGBA{0.0f, 0.2f, 0.4f, 1.0f});
    m_deviceContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void AECore::D3D11GLI::Swap() {
    m_swapChain->Present(0, 0);
}

void AECore::D3D11GLI::D3DCreateCall(HRESULT hresult, std::string failInfo) {
    if (FAILED(hresult)) {
        std::cout << failInfo << std::endl;
        PrintHResult(hresult);
    }
}

//Only Creates index buffers
std::shared_ptr<IBuffer> AECore::D3D11GLI::CreateBuffer(const void* data, size_t count, size_t stride, AEngine::Graphics::BufferType bufferType) {
    return std::make_shared<DX11_Buffer>(m_device.Get(), m_deviceContext.Get(), count, stride, data, bufferType);
}

std::shared_ptr<DX11ShaderObject> AECore::D3D11GLI::CreateShaderObject(const void* data, size_t dataSize, std::string entryPoint, std::string shaderTarget) {
    return std::make_shared<DX11ShaderObject>(m_deviceContext.Get(), m_device.Get(), data, dataSize, entryPoint, shaderTarget);
}

std::shared_ptr<AE::Core::Graphics::IShaderResourceView> AECore::D3D11GLI::CreateShaderResourceView(const std::shared_ptr<AE::Core::Graphics::ITextureResource> textureResource) {
    return std::make_shared<DX11ShaderResourceView>(
            m_deviceContext, 
            m_device, 
            static_cast<ID3D11Resource*>(textureResource->Get()), 
            D3D11_SRV_DIMENSION_TEXTURE2D
    );
}

std::shared_ptr<AE::Core::Graphics::ISampler> AECore::D3D11GLI::CreateSampler() {
    return std::make_shared<DX11Sampler>(
            m_deviceContext,
            m_device
    );
}

std::shared_ptr<AE::Core::Graphics::ITextureResource> AECore::D3D11GLI::CreateTextureResource(const TextureCreateInfo& createInfo) {
    return std::make_shared<DX11TextureResource>(
        m_deviceContext, m_device, createInfo
        );
}

std::shared_ptr<IFragmentShader> AECore::D3D11GLI::CreateFragmentShader(const void* data, size_t dataSize) {
    auto shaderObject = CreateShaderObject(data, dataSize, "PShader", "ps_4_0");

    //TODO: Error check here, creation could fail look into tactics for RAII failure error should have alread been logged
    //Should probably attach a standard pink shader
    return std::make_shared<DX11FragmentShader>(m_deviceContext, m_device, shaderObject);

}


//Binding
void AECore::D3D11GLI::BindBuffer(const std::shared_ptr<IBuffer>& ib) {
    ib->Bind();
}

void AECore::D3D11GLI::ShutDown() {

}

std::shared_ptr<IVertexShader> AECore::D3D11GLI::CreateVertexShader(std::shared_ptr<IShader>) {
    return nullptr;
};