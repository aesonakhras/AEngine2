#pragma once
#include "DeviceCreateInfo.h"

#include <d3d11.h>

namespace AECore {
	class IGLI {
	public:
		//Temp Solution, ensure that any API specific code is out of this soon

		virtual ID3D11Device*  GetDevice() = 0;
		virtual ID3D11DeviceContext* GetDeviceContext() = 0;

		virtual void Init(AECore::DeviceCreateInfo info) = 0;
		virtual void ShutDown() = 0;

		virtual void Clear() = 0;
		virtual void Swap() = 0;

		virtual ~IGLI() {};
	};
}