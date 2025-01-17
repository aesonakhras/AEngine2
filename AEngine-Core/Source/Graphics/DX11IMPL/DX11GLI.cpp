#include <iostream>
#include <comdef.h>
#include "d3dcompiler.h"


#include "Graphics/DeviceCreateInfo.h"

#include "DX11GLI.h"
#include "DX11Buffer.h"
#include "DX11ShaderObject.h"
#include "DX11Sampler.h"
#include "DX11ShaderResourceView.h"
#include "DX11TextureResource.h"
#include "DX11FragmentShader.h"
#include "DX11VertexShader.h"


using namespace AE::Graphics;

void DX11GLI::PrintHResult(HRESULT result) {
    _com_error err(result);
    LPCTSTR errMsg = err.ErrorMessage();

    std::wcout << errMsg << std::endl;
}

void DX11GLI::Init(const DeviceCreateInfo& info) {
    HRESULT hr;
    
    //TODO: Better error handling
    createDevice();
    createSwapChainAndBackBuffer(info);
    setupViewport(info);
    createRasterStates();
    

    //render texture creation
    D3D11_TEXTURE2D_DESC renderTextureDesc = {};
    renderTextureDesc.Width = info.ScreenWidth;
    renderTextureDesc.Height = info.ScreenHeight;
    renderTextureDesc.MipLevels = 1;
    renderTextureDesc.ArraySize = 1;
    renderTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderTextureDesc.SampleDesc.Count = 1;
    renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    renderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    ID3D11Texture2D* renderTexture = nullptr;
    hr = m_device->CreateTexture2D(&renderTextureDesc, nullptr, &renderTexture);
    if (FAILED(hr)) {
        std::cerr << "Failed to create render texture\n";
    }

    m_renderTargetView = nullptr;
    hr = m_device->CreateRenderTargetView(renderTexture, nullptr, &m_renderTargetView);
    if (FAILED(hr)) {
        std::cerr << "Failed to create render texture view\n";
    }

    m_shaderResourceView = nullptr;
    hr = m_device->CreateShaderResourceView(renderTexture, nullptr, &m_shaderResourceView);
    if (FAILED(hr)) {
        std::cerr << "Failed to create render texture resource view\n";
    }
    
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = info.ScreenWidth;
    depthStencilDesc.Height = info.ScreenHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    ID3D11Texture2D* depthStencilBuffer = nullptr;
    hr = m_device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
    if (FAILED(hr)) {
        std::cerr << "Failed to create depth stencil buffer\n";
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    m_depthStencilView = nullptr;
    hr = m_device->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &m_depthStencilView);
    if (FAILED(hr)) {
        std::cerr << "Failed to create depth stencil view\n";
    }

    m_samplerState = nullptr;

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerState);
    if (FAILED(hr)) {
        std::cerr << "Failed to create sampler state";
    }

    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    setupDepthStencilState();

    setUpScreenQuad();

    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetProcessDPIAware();
}

void DX11GLI::createDevice() {
    D3D_FEATURE_LEVEL featureLevel{};

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &m_device,
        &featureLevel,
        &m_deviceContext
    );

    if (FAILED(hr)) {
        std::cerr << "Failed to create Direct3D 11 device.\n";
        return;
    }
}

void DX11GLI::createSwapChainAndBackBuffer(const DeviceCreateInfo& info) {
    DXGI_SWAP_CHAIN_DESC desc = {};

    desc.BufferCount = 2;
    desc.BufferDesc.Width = info.ScreenWidth;
    desc.BufferDesc.Height = info.ScreenHeight;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = info.Window.GetWindowHandle<HWND>();
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    m_device->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice);

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    dxgiDevice->GetAdapter(&adapter);

    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    adapter->GetParent(__uuidof(IDXGIFactory), &factory);

    HRESULT hr = factory->CreateSwapChain(m_device.Get(), &desc, &m_swapChain);
    if (FAILED(hr)) {
        std::cerr << "Failed to create swap chain.\n";
        return;
    }

    //now lets set up the render target
    ID3D11Texture2D* pBackBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    D3DCreateCall(m_device->CreateRenderTargetView(pBackBuffer, NULL, m_backBuffer.GetAddressOf()), "failed to Create render target.");
    pBackBuffer->Release();
}

void DX11GLI::setupDepthStencilState() {
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
    D3DCreateCall(m_device->CreateDepthStencilState(&dsDesc, m_defaultDepthStencilState.GetAddressOf()), "Unable to create Depth Stencil state");
    m_deviceContext->OMSetDepthStencilState(m_defaultDepthStencilState.Get(), 0);


    //create the skyboxstate 
    D3D11_DEPTH_STENCIL_DESC skyboxDSDesc = {};
    skyboxDSDesc.DepthEnable = true;
    skyboxDSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // Prevent overwriting depth buffer
    skyboxDSDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;      // Allow fragments farthest from the camera
    skyboxDSDesc.StencilEnable = false;

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

    
    HRESULT hr = m_device->CreateDepthStencilState(&skyboxDSDesc, m_skyBoxStencilState.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create depth stencil state for skybox.\n";
    }
    
}

void DX11GLI::setupViewport(const DeviceCreateInfo& info) {
    D3D11_VIEWPORT viewport{};
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = info.ScreenWidth;
    viewport.Height = info.ScreenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_deviceContext->RSSetViewports(1, &viewport);
}

void DX11GLI::createRasterStates() {
    //set up the raster state
    D3D11_RASTERIZER_DESC  defaultRasterDesc{};

    defaultRasterDesc.FillMode = D3D11_FILL_SOLID;
    defaultRasterDesc.CullMode = D3D11_CULL_BACK;
    defaultRasterDesc.FrontCounterClockwise = FALSE;
    defaultRasterDesc.DepthBias = 0;
    defaultRasterDesc.DepthBiasClamp = 0;
    defaultRasterDesc.SlopeScaledDepthBias = 0;
    defaultRasterDesc.DepthClipEnable = TRUE;
    defaultRasterDesc.ScissorEnable = FALSE;
    defaultRasterDesc.MultisampleEnable = TRUE;
    defaultRasterDesc.AntialiasedLineEnable = TRUE;

    Microsoft::WRL::ComPtr < ID3D11RasterizerState> DefaultrasterState;
    D3DCreateCall(
        m_device->CreateRasterizerState(
            &defaultRasterDesc,
            DefaultrasterState.GetAddressOf()
        ),
        "Unable to create Raster State"
    );

    //TODO: Error code

    
    rasterStates["Default"] = DefaultrasterState;

    Microsoft::WRL::ComPtr < ID3D11RasterizerState> quadRasterState;
    D3D11_RASTERIZER_DESC quadRasterDesc = defaultRasterDesc;
    quadRasterDesc.CullMode = D3D11_CULL_NONE;
    quadRasterDesc.DepthClipEnable = FALSE;

    D3DCreateCall(
        m_device->CreateRasterizerState(
            &quadRasterDesc,
            quadRasterState.GetAddressOf()
        ),
        "Unable to create Raster State"
    );

    rasterStates["ScreenQuad"] = quadRasterState;

}
void DX11GLI::setRasterState(std::string state) {
    auto newRasterState = rasterStates[state];

    m_deviceContext->RSSetState(newRasterState.Get());
}

void DX11GLI::StartFrame() {
    setRasterState("Default");
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, RGBA{ 0.1f, 0.2f, 0.3f, 1.0f });
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    
}

void DX11GLI::PresentFrame() {
    setRasterState("ScreenQuad");
    m_deviceContext->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), nullptr);

    UINT stride = sizeof(ScreenQuadVertex);
    UINT offset = 0;
    m_quadVertexBuffer->Bind(0);

    // Bind index buffer
    m_quadIndexBuffer->Bind(0);

    m_screenQuadFragmentShader->Bind();
    m_screenQuadVertexShader->Bind();

    // Bind the shader resource view of the render texture (created in the previous steps)
    m_deviceContext->PSSetShaderResources(0, 1, &m_shaderResourceView);
    m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);

    m_deviceContext->DrawIndexed(6, 0, 0);

    // Unbind the render texture from the shader resource slot
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    m_deviceContext->PSSetShaderResources(0, 1, nullSRV);

    // Present the back buffer
    m_swapChain->Present(1, 0);

    // Reset the rasterizer and render target states (optional)
    setRasterState("Default");
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
};

void DX11GLI::D3DCreateCall(HRESULT hresult, std::string failInfo) {
    if (FAILED(hresult)) {
        //I consider this fatal at this point
        std::cout << failInfo << std::endl;
        PrintHResult(hresult);
    }
}

void DX11GLI::Draw(unsigned int size) {
    m_deviceContext->DrawIndexed(size, 0, 0);
}

//Only Creates index buffers
std::shared_ptr<IBuffer> DX11GLI::CreateBuffer(const void* data, size_t count, size_t stride, BufferType bufferType) {
    return std::make_shared<DX11Buffer>(m_device.Get(), m_deviceContext.Get(), count, stride, data, bufferType);
}

std::shared_ptr<DX11ShaderObject> DX11GLI::CreateShaderObject(const void* data, size_t dataSize, std::string entryPoint, std::string shaderTarget) {
    return std::make_shared<DX11ShaderObject>(m_deviceContext.Get(), m_device.Get(), data, dataSize, entryPoint, shaderTarget);
}

std::shared_ptr<AE::Graphics::IShaderResourceView> DX11GLI::CreateShaderResourceView(const std::shared_ptr<AE::Graphics::ITextureResource> textureResource) {
    auto dx11ShaderResourceView = std::static_pointer_cast<DX11TextureResource>(textureResource);

    return std::make_shared<DX11ShaderResourceView>(
            m_deviceContext, 
            m_device, 
            dx11ShaderResourceView
    );
}

std::shared_ptr<AE::Graphics::ISampler> DX11GLI::CreateSampler() {
    return std::make_shared<DX11Sampler>(
            m_deviceContext,
            m_device
    );
}

std::shared_ptr<AE::Graphics::ITextureResource> DX11GLI::CreateTextureResource(const TextureCreateInfo& createInfo) {
    return std::make_shared<DX11TextureResource>(
        m_deviceContext, m_device, createInfo
        );
}

std::shared_ptr<IVertexShader> DX11GLI::CreateVertexShader(const void* data, size_t dataSize, const std::vector<VertexAttribute>& attributes) {
    auto shaderObject = CreateShaderObject(data, dataSize, "VShader", "vs_4_0");

    //TODO: Error check here, creation could fail look into tactics for RAII failure error should have alread been logged
    //Should probably attach a standard pink shader
    return std::make_shared<DX11VertexShader>(m_deviceContext, m_device, shaderObject, attributes);
};

std::shared_ptr<IFragmentShader> DX11GLI::CreateFragmentShader(const void* data, size_t dataSize) {
    auto shaderObject = CreateShaderObject(data, dataSize, "PShader", "ps_4_0");

    //TODO: Error check here, creation could fail look into tactics for RAII failure error should have alread been logged
    //Should probably attach a standard pink shader
    return std::make_shared<DX11FragmentShader>(m_deviceContext, m_device, shaderObject);

}

void DX11GLI::RecompileVertexShader(const void* data, size_t dataSize, std::shared_ptr<IVertexShader>& vertexShader) {
    auto dx11vertexShader = std::static_pointer_cast<DX11VertexShader>(vertexShader);

    //recompile the shdaer itself first
    dx11vertexShader->Shader->Compile(data, dataSize, "VShader", "vs_4_0");

    //recompile the vertex shader and layout, which is currently in the vertex shader
    dx11vertexShader->Recreate();
}

void DX11GLI::RecompileFragmentShader(const void* data, size_t dataSize, std::shared_ptr <IFragmentShader>& fragmentShader) {
    auto dx11fragmentShader = std::static_pointer_cast<DX11FragmentShader>(fragmentShader);

    //recompile the shdaer itself first
    dx11fragmentShader->Shader->Compile(data, dataSize, "PShader", "ps_4_0");

    //recompile the vertex shader and layout, which is currently in the vertex shader
    dx11fragmentShader->Recreate();
}

void DX11GLI::ChangeDepthState(bool isSkybox) {
    if (isSkybox) {
        m_deviceContext->OMSetDepthStencilState(m_skyBoxStencilState.Get(), 0);
    }
    else {
        m_deviceContext->OMSetDepthStencilState(m_defaultDepthStencilState.Get(), 0);
    }
}

//Binding
void DX11GLI::BindBuffer(const std::shared_ptr<IBuffer>& ib) {
    ib->Bind(0);
}


void DX11GLI::setUpScreenQuad() {
    m_quadVertexBuffer = std::make_shared<DX11Buffer>(
        m_device.Get(),
        m_deviceContext.Get(),
        4,
        sizeof(ScreenQuadVertex),
        m_screenQuadVertices,
        BufferType::Vertex
        );

    m_quadIndexBuffer = std::make_shared<DX11Buffer>(
        m_device.Get(),
        m_deviceContext.Get(),
        6,
        sizeof(unsigned int),
        m_screenQuadIndices,
        BufferType::Index
        );


    //create the shader object
    m_screenQuadVertexShader = CreateVertexShader (
        ScreenQuadVertexShader.data(),
        ScreenQuadVertexShader.length(),
        ScreenQuadVertexDescription::Get()
    );

    m_screenQuadFragmentShader = CreateFragmentShader(
        ScreenQuadFragmentShader.data(),
        ScreenQuadFragmentShader.length()
    );

}


void DX11GLI::ShutDown() {

}