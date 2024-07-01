#pragma once
#include "DeviceCreateInfo.h"
#include "IndexBuffer.h"

#include <d3d11.h>


//TODO: Must refactor, this is a D3D11 class.  need to determine abstraction
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

		//Creation
		virtual IIndexBuffer CreateIndexBuffer(const void* data, size_t count, size_t stride) = 0;

		//Binding
		virtual void BindBuffer(const std::shared_ptr<IIndexBuffer>& ib) = 0;

		virtual ~IGLI() {};
	};
}