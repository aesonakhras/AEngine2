#pragma once
#include <d3d11.h>
#include <string>

#include "IGLI.h"

#include <wrl/client.h>

namespace AECore {
	class D3D11GLI : public IGLI {

	public:

		///////////////////////////From the IGLI
		virtual void Init(AECore::DeviceCreateInfo info) final override;
		virtual void ShutDown() final override;

		virtual ID3D11Device* GetDevice() final override;
		virtual ID3D11DeviceContext* GetDeviceContext() final override;
		virtual void Clear() final override;
		virtual void Swap() final override;
		///////////////////////////End of the IGLI functions

	private:
		void setupDepthStencilState();
		void D3DCreateCall(HRESULT hresult, std::string failInfo);
		void PrintHResult(HRESULT result);

		ID3D11Texture2D* CreateTextureD3D(void* data, unsigned int height, unsigned int width, unsigned int miplevel,
			DXGI_FORMAT format, unsigned int sampleCount, unsigned int BindFlags);

		//TODO: Find another solution for this
		typedef float RGBA[4];

		//debugging

		Microsoft::WRL::ComPtr <ID3D11Device> m_device = nullptr;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext = nullptr;
		Microsoft::WRL::ComPtr <ID3D11Texture2D> m_depthTexture = nullptr;
		Microsoft::WRL::ComPtr <IDXGISwapChain> m_swapChain = nullptr;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_backBuffer = nullptr;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> m_pDSV = nullptr;
	};
}