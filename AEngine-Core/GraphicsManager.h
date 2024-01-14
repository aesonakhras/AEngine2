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

#include "IGLI.h"

namespace AECore {
	class GraphicsManager {

	public:
		GraphicsManager() {/* intentianlly left blank Init will initialize, this is to allow start up to be done in programmer order*/ };
		~GraphicsManager() {/*same as above*/ }

		bool Initialize(AECore::DeviceCreateInfo info);
		void DrawFrame(std::vector<StaticMesh*> meshes, DirectX::XMMATRIX VP);
		void ShutDown();

		//TODO: TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		//CreateBuffer
		

	private:
		void DrawMesh(const StaticMesh& mesh, DirectX::XMMATRIX VP);

		std::unique_ptr<IGLI> m_GLI = nullptr;

		UINT m_stride = sizeof(AEngineVertexTypes::VERTEX);
		UINT m_offset = 0;
	};
}