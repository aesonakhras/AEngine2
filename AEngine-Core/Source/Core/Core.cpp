#include "Core.h"

#include <iostream>
#include <Windows.h>

#include <d3d11.h>
#include "d3dcompiler.h"
#include <DirectXMath.h>
#include <dxgidebug.h>

//for fun
#include <memory.h>

#include <vector>

//This is for Debugging
#include <comdef.h>
#include <filesystem>

//AEngine Specific
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexShader.h"
#include "../FragmentShader.h"

#include "../StaticMesh.h"
#include "../Camera.h"

#include "../Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//TODO: Lmao what is this
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct VERTEX {
    DirectX::XMFLOAT3 POS;
    DirectX::XMFLOAT2 UV;
};



//Notes:
//The D3DX (D3DX 9, D3DX 10, and D3DX 11) utility library is deprecated

namespace Core {
    IDXGISwapChain* swapChain = nullptr;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    ID3D11RenderTargetView* backBuffer = nullptr;
    //std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
    std::shared_ptr<IndexBuffer> indexBuffer = nullptr;

    std::shared_ptr<VertexShader> vertexShader = nullptr;
    std::shared_ptr<FragmentShader> fragmentShader = nullptr;

    std::vector<StaticMesh*> meshes;

    std::shared_ptr<Texture> depthTexture = nullptr;

    ID3D11DepthStencilView* pDSV;

    Camera camera = { {0.0f, 0.0f, -1.0f, 0.0f}, 1.0472f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.01f, 100.0f };


    //This is a quad
    VERTEX QuadVerticies[] =
    {
        {  DirectX::XMFLOAT3(- 1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
        { DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
        { DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
        { DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
    };

    unsigned int QuadIndicies[] = { 
        0,1,2, 1,3,2,
    };

    VERTEX CubeVerticies[] = {
            {DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)},
            {DirectX::XMFLOAT3(1.0f,  -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)},
            {DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
            {DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)},

            // Back Face
            {DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)},
            {DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),DirectX::XMFLOAT2(1.0f, 1.0f)},
            {DirectX::XMFLOAT3(-1.0f,  1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
            {DirectX::XMFLOAT3(1.0f,  1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)},
    };

    unsigned int CubeIndicies[] = {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };

    VERTEX TriangleVerticies[] =
    {
        {DirectX::XMFLOAT3(0.0f, 0.73f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)},
        {DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)},
        {DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)}
    };

    unsigned int TriangleIndicies[] = { 0, 1, 2 };


    void PrintHResult(HRESULT result) {
        _com_error err(result);
        LPCTSTR errMsg = err.ErrorMessage();

        std::wcout << errMsg << std::endl;
    }

	void PrintHelloWorld()
	{
		std::cout << "Hello World!\n";
	}

    void printCWD() {
        std::filesystem::path cwd = std::filesystem::current_path();
        std::cout << "CWD: " << cwd << std::endl;
    }

    //Code for window is mostly taken from http://www.directxtutorial.com/ with modifications
    //to integrate with this project

    // this is the main message handler for the program
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        // sort through and find what code to run for the message given
        switch (message)
        {
            // this message is read when the window is closed
        case WM_DESTROY:
        {
            // close the application entirely
            PostQuitMessage(0);
            return 0;
        } break;
        }

        // Handle any messages the switch statement didn't
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void SetUpPipeline() {
        vertexShader = std::make_shared<VertexShader>(deviceContext, device, L"shaders.shader", DirectX::XMFLOAT4{1.0f, 0.0f, 0.0f, 0.0f});

        auto vertexShader2 = std::make_shared<VertexShader>(deviceContext, device, L"shaders.shader", DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 0.0f });

        fragmentShader = std::make_shared<FragmentShader>(deviceContext, device, L"shaders.shader");

        //send data to the GPU
        //define the triangle


        //VERTEX OurVertices2[] =
        //{
        //    {-0.5f, 0.5f, 0.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
        //    {0.0f, -0.5f, 0.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        //    {-1.0f, -0.5f, 0.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
        //};

        //setup the index buffer
        //just assume it worked for now
        

        //vertexBuffer = std::make_shared<VertexBuffer>(deviceContext, device, (void*)OurVertices, (unsigned int)(3 * sizeof(VERTEX)));
        indexBuffer = std::make_shared<IndexBuffer>(deviceContext, device, (void*)TriangleIndicies, sizeof(unsigned int) * 3);

        auto vertexBuffer1 = std::make_shared<VertexBuffer>(deviceContext, device, (void*)TriangleVerticies, (unsigned int)(3 * sizeof(VERTEX)));
        //auto vertexBuffer2 = std::make_shared<VertexBuffer>(deviceContext, device, (void*)OurVertices2, (unsigned int)(3 * sizeof(VERTEX)));

        std::cout << "Cube Vertex Size" << sizeof(QuadVerticies) / sizeof(QuadVerticies[0]) << std::endl;

        std::cout << "Cube index Size" << sizeof(QuadIndicies) / sizeof(QuadIndicies[0]) << std::endl;

        //auto vertexBuffer2 = std::make_shared<VertexBuffer>(deviceContext, device, (void*)CubeVerticies, sizeof(VERTEX) * (sizeof(CubeVerticies)/sizeof(CubeVerticies[0])));
        //auto indexBuffer2 = std::make_shared<IndexBuffer>(deviceContext, device, (void*)CubeIndicies, sizeof(unsigned int) * (sizeof(CubeIndicies) / sizeof(CubeIndicies[0])));

        auto vertexBuffer2 = std::make_shared<VertexBuffer>(deviceContext, device, (void*)CubeVerticies, sizeof(VERTEX) * (sizeof(CubeVerticies)/sizeof(CubeVerticies[0])));
        auto indexBuffer2 = std::make_shared<IndexBuffer>(deviceContext, device, (void*)CubeIndicies, sizeof(unsigned int) * (sizeof(CubeIndicies) / sizeof(CubeIndicies[0])));
        Transform transform{
            {0.0f, 0.0f, 1.0f, 0.0f}, //pos
            {0.0f, 0.0f, 0.0f, 0.0f}, //rot
            {1.0f, 1.0f, 1.0f}  //scale
        };

        Transform transform2 {
            {10.0f, 10.0f, 100.0f, 0.0f}, //pos
            {0.0f, 0.0f, 0.0f, 0.0f}, //rot
            {1.5f, 1.5f, 1.5f}  //scale
        };

        
        meshes.push_back(new StaticMesh{vertexBuffer2, indexBuffer2, vertexShader, fragmentShader, transform, deviceContext});
        //meshes.push_back(new StaticMesh{ vertexBuffer1, indexBuffer, vertexShader2, fragmentShader, transform2, deviceContext });
        //create 2 meshes for drawing

        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void setupDepthStencilState() {
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
        device->CreateDepthStencilState(&dsDesc, &pDSState);
    }

    void InitD3D(HWND hwnd) {
        DXGI_SWAP_CHAIN_DESC desc;

        //TODO: Is this very c++ like
        ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

        desc.BufferCount = 1;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
        desc.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.OutputWindow = hwnd;
        desc.SampleDesc.Count = 4;
        desc.Windowed = TRUE;
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        auto result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &desc, &swapChain, &device, NULL, &deviceContext);

        if (FAILED(result)) {
            std::cout << "DX11 has failed to start\n";
        }

        //Depth
        setupDepthStencilState();

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));

        descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        descDSV.Texture2D.MipSlice = 0;

        //get the texure
        depthTexture = std::make_shared<Texture>(device, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT, 1,
                                                DXGI_FORMAT_D32_FLOAT_S8X24_UINT, 4, D3D11_BIND_DEPTH_STENCIL);

        // Create the depth stencil view
        
        HRESULT hr = device->CreateDepthStencilView(depthTexture->GetTexture(), // Depth stencil texture
            &descDSV, // Depth stencil desc
            &pDSV);  // [out] Depth stencil view

        if (FAILED(hr)) {
            std::cout << "Depth Stencil Fail\n";
            PrintHResult(hr);
        }

        //now lets set up the render target
        ID3D11Texture2D* pBackBuffer;
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

        device->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
        pBackBuffer->Release();

        deviceContext->OMSetRenderTargets(1, &backBuffer, pDSV);

        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = SCREEN_WIDTH;
        viewport.Height = SCREEN_HEIGHT;
        deviceContext->RSSetViewports(1, &viewport);


        //set up the raster state
        D3D11_RASTERIZER_DESC  rasterDesc;

        rasterDesc.FillMode = D3D11_FILL_SOLID ,
        rasterDesc.CullMode = D3D11_CULL_BACK,
        rasterDesc.FrontCounterClockwise = FALSE;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0;
        rasterDesc.SlopeScaledDepthBias = 0;
        rasterDesc.DepthClipEnable = FALSE;
        rasterDesc.ScissorEnable = FALSE;
        rasterDesc.MultisampleEnable = TRUE;
        rasterDesc.AntialiasedLineEnable = TRUE;

        ID3D11RasterizerState* rasterState;
        hr = device->CreateRasterizerState(&rasterDesc, & rasterState);

        if (FAILED(hr)) {
            std::cout << "Raster state Fail\n";
            PrintHResult(hr);
        }

        deviceContext->RSSetState(rasterState);
    }

    void CleanD3D() {
        swapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

        swapChain->Release();
        backBuffer->Release();
        device->Release();
        deviceContext->Release();


        //TODO: Come back to this
        //IDXGIDebug* debugDev;
        //HRESULT hr = DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&debugDev);

        //debugDev->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);
    }

    //epic in namespace decleration
    //Need to find a math solution
    typedef float RGBA[4];

    float y = 0;

    float zPos = 6;

    //oooh not very good, this is quite stinky
    //but for now we will pass it references to the things that we need
    void renderFrame() {
        
        deviceContext->ClearRenderTargetView(backBuffer, RGBA{ 0.0f, 0.2f, 0.4f, 1.0f });
        deviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);


        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        
        y += 0.00005f;
        if (y > 6.28) {
            y = 0.0f;
        }

        int i = 0;
        for (auto mesh : meshes) {
            if (i == 0) {
                Transform transform{
                    {0.0f, 0.0f, zPos, 1.0f}, //pos
                    {0.0f, y, 0.0f, 1.0f}, //rot
                    {1.0f, 1.0f, 1.0f, 1.0f}  //scale
                };

                mesh->SetTransform(transform);
            }
            i++;
            mesh->Draw(stride, camera.GetVP());
        }

        // draw the vertex buffer to the back buffer
        //deviceContext->Draw(3, 0);

        swapChain->Present(0, 0);
    }

    void cleanUp() {
        //elete vertexBuffer;
    }

    //hahaha lazy
    Texture* texture;

    void textureSetup() {
        int width = 0;
        int height = 0;
        int channels = 0;
        const char* filename = "map.png";

        unsigned char* img = stbi_load(filename, &width, &height, &channels, 0);

        std::cout << width << " , " << height << std::endl;

        texture = new Texture(device, img, width, height, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, D3D11_BIND_SHADER_RESOURCE);

        ID3D11ShaderResourceView* shaderResourceView;
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc;
        ZeroMemory(&shaderViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        shaderViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        shaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

        device->CreateShaderResourceView(texture->GetTexture(), NULL, &shaderResourceView);

        ID3D11SamplerState* pSampler = nullptr;
        // Create the sample state
        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory(&sampDesc, sizeof(sampDesc));
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
        device->CreateSamplerState(&sampDesc, &pSampler);

        deviceContext->PSSetShaderResources(0, 1, &shaderResourceView);
        deviceContext->PSSetSamplers(0, 1, &pSampler);

        free(img);
    }


	void SetupWindow() {
        // the handle for the window, filled by a function
        HWND hWnd;
        // this struct holds information for the window class
        WNDCLASSEX wc;

        // clear out the window class for use
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        // fill in the struct with the needed information
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;

        //Could be a problem area only works if statically linked
        wc.hInstance = GetModuleHandle(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        // register the window class
        RegisterClassEx(&wc);

        //be able to set the client size properly
        RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        // create the window and use the result as the handle
        hWnd = CreateWindowEx(NULL,
            L"WindowClass1",    // name of the window class
            L"Our First Windowed Program",   // title of the window
            WS_OVERLAPPEDWINDOW,    // window style
            300,    // x-position of the window
            300,    // y-position of the window
            rect.right - rect.left,    // width of the window
            rect.bottom - rect.top,    // height of the window
            NULL,    // we have no parent window, NULL
            NULL,    // we aren't using menus, NULL
            wc.hInstance,    // application handle
            NULL);    // used with multiple windows, NULL

        // display the window on the screen
        ShowWindow(hWnd, SW_NORMAL);

        // this struct holds Windows event messages
        MSG msg;

        ///////////////////////////////////Set Up Direct X related stuff////////////////////////////////////



        InitD3D(hWnd);
        SetUpPipeline();
        textureSetup();




        //test to see if free image works

        // wait for the next message in the queue, store the result in 'msg'
        while (TRUE)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                // translate keystroke messages into the right format
                TranslateMessage(&msg);

                // send the message to the WindowProc function
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT)
                    break;
            }
            else {
                //Handle the game logic
                renderFrame();
            }
        }
        CleanD3D();
	}

    void Start() {
        std::cout << "Engine Boot\n";

        SetupWindow();

        std::cin.get();
    }
}