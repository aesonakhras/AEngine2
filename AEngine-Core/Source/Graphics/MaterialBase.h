#pragma once

#include <memory>
#include <string>

//TODO: This is for the Uniform Description which will be moved
//to it's own file once I get this refactor working
#include "Core/Types.h"

namespace AE::Graphics {

	struct UniformDescriptionElement {
		std::string name;
		uint32 size;  //size of data

		UniformDescriptionElement(
			const std::string& name,
			size_t size
		)
		: name(name),
		size(size) { }
	};

	struct UniformLayoutDescription {
		UniformLayoutDescription() = delete;

		UniformLayoutDescription(const std::vector<UniformDescriptionElement>& uniformLayout) {
			Size = 0;
			
			for (const auto& uniform : uniformLayout) {
					UniformLayout[uniform.name] = Size;
					Size += uniform.size;
			}
		}

		std::unordered_map <std::string, uint32> UniformLayout{};
		uint32 Size;
	};

	//forward declares
	class IVertexShader;
	class IFragmentShader;

	struct MaterialBase {
		std::shared_ptr<IVertexShader> VertexShader;
		std::shared_ptr<IFragmentShader> FragmentShader;
		UniformLayoutDescription UniformDescription;

		MaterialBase(
			std::shared_ptr<IVertexShader> vertexShader,
			std::shared_ptr<IFragmentShader> fragmentShader,
			UniformLayoutDescription uniformDescription
		) : VertexShader(vertexShader),
			FragmentShader(fragmentShader),
			UniformDescription(std::move(uniformDescription)){ }
	};
}