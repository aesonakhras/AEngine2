#include "Core.h"

#include <iostream>
#include <Windows.h>

#include <d3d11.h>
#include "d3dcompiler.h"
#include <DirectXMath.h>

//TODO: Lmao what is this
#pragma comment(lib, "d3d11.lib")

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


struct VERTEX {
    //TODO: What is going on with this float here
    //Why is this not a XMFloat3?? HUH
    FLOAT X, Y, Z;
    DirectX::XMFLOAT4 color;
};

//Notes:
//The D3DX (D3DX 9, D3DX 10, and D3DX 11) utility library is deprecated

namespace Core {
    IDXGISwapChain* swapChain = nullptr;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    ID3D11RenderTargetView* backBuffer = nullptr;
    ID3D11Buffer* pVBuffer = nullptr;

    ID3D11VertexShader* pVS;
    ID3D11PixelShader* pPS;

	void PrintHelloWorld()
	{
		std::cout << "Hello World!\n";
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
        ID3D10Blob *VS;
        ID3D10Blob *PS;

        //TODO: Apparently I need to use another function than this
        //for times sake I will go ahead and use this and convert it later
        D3DCompileFromFile(L"Shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
        D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);

        device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
        device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
        

        //define the triangle
        VERTEX OurVertices[] =
        {
            {0.0f, 0.5f, 0.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
            {0.45f, -0.5, 0.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
            {-0.45f, -0.5f, 0.0f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
        };

        //set up the vertex buffer
        D3D11_BUFFER_DESC bd;

        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX) * 3;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        device->CreateBuffer(&bd, NULL, &pVBuffer);

        D3D11_MAPPED_SUBRESOURCE ms;

        //TODO: Either disable or move on
        deviceContext->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, OurVertices, sizeof(OurVertices));
        deviceContext->Unmap(pVBuffer, NULL);
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

        auto result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &desc, &swapChain, &device, NULL, &deviceContext);

        if (FAILED(result)) {
            std::cout << "DX11 has failed to start\n";
        }

        //now lets set up the render target
        ID3D11Texture2D* pBackBuffer;
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

        device->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
        pBackBuffer->Release();

        deviceContext->OMSetRenderTargets(1, &backBuffer, NULL);

        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = SCREEN_WIDTH;
        viewport.Height = SCREEN_HEIGHT;
        deviceContext->RSSetViewports(1, &viewport);
    }

    void CleanD3D() {
        swapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

        pVS->Release();
        pPS->Release();

        swapChain->Release();
        backBuffer->Release();
        device->Release();
        deviceContext->Release();
    }

    //epic in namespace decleration
    //Need to find a math solution
    typedef float RGBA[4];

    //oooh not very good, this is quite stinky
    //but for now we will pass it references to the things that we need
    void renderFrame() {
        deviceContext->ClearRenderTargetView(backBuffer, RGBA{ 0.0f, 0.2f, 0.4f, 1.0f });

        swapChain->Present(0, 0);
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