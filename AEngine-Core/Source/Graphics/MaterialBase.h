#pragma once

#include <memory>
#include <string>
#include "Core/Types.h"

namespace AE::Graphics {

	struct UniformDescription {
		std::string name;
		uint32 size;  //size of data
	};
	//contains UBO layout
	//vertexShdaer
	//fragmentShader


	//forward declares
	class IVertexShader;
	class IFragmentShader;

	struct MaterialBase {
		std::shared_ptr<IVertexShader> VertexShader;
		std::shared_ptr<IFragmentShader> FragmentShader;
		std::unordered_map <std::string, uint32> UniformDataLayout;

		uint32 UniformSize;

		MaterialBase(
			std::shared_ptr<IVertexShader> vertexShader,
			std::shared_ptr<IFragmentShader> fragmentShader,
			const std::vector<UniformDescription>& uniformlayout
		) : VertexShader(vertexShader),
			FragmentShader(fragmentShader) {
			//uniform buffer all we need to do is specify a size?

			uint32 totalSize = 0;

			for (const auto& uniform : uniformlayout) {
				UniformDataLayout[uniform.name] = totalSize;
				totalSize += uniform.size;
			}

			UniformSize = totalSize;
		}
	};
}