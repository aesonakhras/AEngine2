#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace AE::Graphics {
	//forward declare
	class IFragmentShader;
	class IVertexShader;
	struct VertexAttribute;

	class ShaderManager {
		std::shared_ptr<IVertexShader> CreateVertexShader(
			const std::string& id,
			const std::string& fileName,
			const std::vector<VertexAttribute>& attributes);

		std::shared_ptr<IVertexShader> GetVertexShader(const std::string& id);

		std::shared_ptr<IFragmentShader> CreateFragmentShader(
			const std::string& id,
			const std::string& fileName
		);

		std::shared_ptr<IFragmentShader> GetFragmentShader(const std::string& id);

	private:
		//id, resource
		static std::unordered_map<std::string, std::shared_ptr<IVertexShader>> m_vertexShaderMap;
		static std::unordered_map<std::string, std::shared_ptr<IFragmentShader>> m_fragmentShaderMap;
	};
}