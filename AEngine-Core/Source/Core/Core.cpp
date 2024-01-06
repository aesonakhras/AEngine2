#include "Core.h"


#include "../GraphicsManager.h"


//I want to see most of the below #include GONE
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
#include "../D3DShaderResource.h"

#include "../Sampler.h"

#include "../FileImporter.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//TempIncludes and other hacks to remove later
#include "../AEngineVertexTypes.h"
using namespace AEngineVertexTypes;

#include "../AEngineConstants.h"
using namespace AEngineConstants;

#include "../DeviceCreateInfo.h"

//TODO: Lmao what is this
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

namespace Core {
    AECore::GraphicsManager g_GraphicsManager;

    std::shared_ptr<VertexShader> vertexShader = nullptr;
    std::shared_ptr<FragmentShader> fragmentShader = nullptr;

    std::vector<StaticMesh*> meshes;

    std::shared_ptr<Texture> depthTexture = nullptr;

    std::shared_ptr<VertexShader> screenQuadVert;
    std::shared_ptr<FragmentShader> screenQuadFrag;

    std::shared_ptr<D3DShaderResource> meshShaderResource;
    std::shared_ptr<D3DShaderResource> screenQuadResource;

    std::shared_ptr<Texture> screenQuadTexture = nullptr;
    ID3D11RenderTargetView* screenQuadRenderTargetView = nullptr;

    //hahaha lazy
    Texture* texture;

    Camera camera = { {0.0f, 0.0f, -1.0f, 0.0f}, 1.0472f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.01f, 100.0f };

    //This is a test for the app core connection
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

    void ImportMesh(std::string fileName) {
        MeshData meshData = FileImporter::ImportMesh(fileName);

        auto vertexBuffer = std::make_shared<VertexBuffer>(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice(), (void*)meshData.vertexData, sizeof(VERTEX) * meshData.vertexCount);
        auto indexBuffer = std::make_shared<IndexBuffer>(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice(), (void*)meshData.indexData, sizeof(unsigned int) * meshData.indexCount);

        Transform transform{
            {0.0f, 0.0f, 1.0f, 0.0f}, //pos
            {0.0f, 0.0f, 0.0f, 0.0f}, //rot
            {1.0f, 1.0f, 1.0f}  //scale
        };

        meshes.push_back(new StaticMesh{ vertexBuffer, indexBuffer, vertexShader, fragmentShader, transform, g_GraphicsManager.GetDeviceContext() });
    }

    void textureSetup() {
        TextureData textureData = FileImporter::ImportTexture(std::string("CatTP.png"));

        texture = new Texture(g_GraphicsManager.GetDevice(), textureData.data, textureData.width, textureData.height, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, D3D11_BIND_SHADER_RESOURCE);
        meshShaderResource = std::make_shared<D3DShaderResource>(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice(), texture->GetTexture(), D3D11_SRV_DIMENSION_TEXTURE2D);
        //screenQuadResource = std::make_shared<D3DShaderResource>(deviceContext, device, screenQuadTexture->GetTexture(), D3D11_SRV_DIMENSION_TEXTURE2D);

        //TODO: Leak much?
        Sampler* sampler = new Sampler(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice());

        meshShaderResource->Bind();
        sampler->Bind();
    }

    void SetupScene() {
        vertexShader = std::make_shared<VertexShader>(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice(), L"shaders.shader", DirectX::XMFLOAT4{1.0f, 0.0f, 0.0f, 0.0f});
        fragmentShader = std::make_shared<FragmentShader>(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice(), L"shaders.shader");

        auto vertexBuffer2 = std::make_shared<VertexBuffer>(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice(), (void*)CubeVerticies, sizeof(VERTEX) * (sizeof(CubeVerticies)/sizeof(CubeVerticies[0])));
        auto indexBuffer2 = std::make_shared<IndexBuffer>(g_GraphicsManager.GetDeviceContext(), g_GraphicsManager.GetDevice(), (void*)CubeIndicies, sizeof(unsigned int) * (sizeof(CubeIndicies) / sizeof(CubeIndicies[0])));

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

        textureSetup();
        ImportMesh(std::string("Cat.obj"));
        //screenQuadVert = std::make_shared<VertexShader>(deviceContext, device, L"ScreenQuad.shader", DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 0.0f });
        //screenQuadFrag = std::make_shared<FragmentShader>(deviceContext, device, L"ScreenQuad.shader");
    }

    //oooh not very good, this is quite stinky
    //but for now we will pass it references to the things that we need
    void renderFrame() {
        static float y = 0;
        float zPos = 1;
        y += 0.0001f;
        if (y > 6.28) {
            y = 0.0f;
        }
        std::cout <<"Y: " << y << std::endl;

        int i = 0;
        for (auto mesh : meshes) {
            if (i == 0) {
                Transform transform{
                    {0.0f, -1.0f, zPos, 1.0f}, //pos
                    {1.5f, 0.0f, 0.0f, 0.0f}, //rot
                    {0.025f, 0.025f, 0.025f, 1.0f}  //scale
                };

                mesh->SetTransform(transform);
            }
            i++;
        }

        g_GraphicsManager.DrawFrame(meshes, camera.GetVP());
    }

    //TODO: Window manager should probably be a thing
    HWND SetupWindow() {
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

        return hWnd;
	}

    void Simulate() {
        // this struct holds Windows event messages
        MSG msg;

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
    }

    void Start() {
        std::cout << "Engine Boot\n";

        //Engine Startup,
        //This should probably be it's own class

        auto hWnd = SetupWindow();

        AECore::DeviceCreateInfo createInfo;
        createInfo.ScreenHeight = SCREEN_HEIGHT;
        createInfo.ScreenWidth = SCREEN_WIDTH;
        createInfo.window = hWnd;

        ///////////////////////////////////Set Up Direct X related stuff////////////////////////////////////
        g_GraphicsManager.Initialize(createInfo);
        SetupScene();
        Simulate();

        g_GraphicsManager.ShutDown();

        std::cin.get();
    }
}