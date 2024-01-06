#pragma once

//TODO: until I implement openGL, Vulkam, or DX12 directX11 will live here
//not the cleanest solution but I literally spent a day trying to design
//a system just guessing what I will need.  Also doing this will avoid a Virtual

//TODO: This needs to be replaced with Legit error code,
//but for not lets just use IOStream until I implement that code
#include <iostream>
#include <d3d11.h>
#include "DeviceCreateInfo.h"
#include "StaticMesh.h"
#include <vector>
#include "AEngineVertexTypes.h"

namespace AECore {
	class GraphicsManager {

	public:
		GraphicsManager() {/* intentianlly left blank Init will initialize, this is to allow start up to be done in programmer order*/ };
		~GraphicsManager() {/*same as above*/ }

		bool Initialize(AECore::DeviceCreateInfo info);
		void DrawFrame(std::vector<StaticMesh*> meshes, DirectX::XMMATRIX VP);
		void ShutDown();

		//TODO: TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D11Device* GetDevice() { return m_device; };
		ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; };

		//CreateBuffer
		

	private:
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_deviceContext = nullptr;
		ID3D11Texture2D* m_depthTexture = nullptr;
		IDXGISwapChain* m_swapChain = nullptr;
		ID3D11RenderTargetView* m_backBuffer = nullptr;
		ID3D11DepthStencilView* m_pDSV = nullptr;

		void setupDepthStencilState();
		void D3DCreateCall(HRESULT hresult, std::string failInfo);

		//internal for this class, do not expose outside
		ID3D11Texture2D* CreateTextureD3D(void* data, unsigned int height, unsigned int width, unsigned int miplevel,
			DXGI_FORMAT format, unsigned int sampleCount, unsigned int BindFlags);
		//TODO: Find another solution for this
		typedef float RGBA[4];
	};
}