#pragma once
#include <vector>

namespace AE::Graphics {
	//forward declares
	struct VertexAttribute;

	class IVertexLayout {
		public:
			virtual void Build(const std::vector<VertexAttribute>& vertexAttributes) = 0;

			virtual void Bind() = 0;
			virtual void UnBind() = 0;
	};
}