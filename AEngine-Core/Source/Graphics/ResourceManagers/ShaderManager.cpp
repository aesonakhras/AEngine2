#include "ShaderManager.h"

#include "Graphics/GraphicsManager.h"
#include "Graphics/IVertexShader.h"
#include "Graphics/IFragmentShader.h"

#include "Core/Debug.h"

using namespace AE::Graphics;

std::unordered_map<std::string, std::shared_ptr<IVertexShader>> ShaderManager::m_vertexShaderMap;
std::unordered_map<std::string, std::shared_ptr<IFragmentShader>> ShaderManager::m_fragmentShaderMap;

std::shared_ptr<IVertexShader> ShaderManager::CreateVertexShader(
	const std::string& id,
	const std::string& fileName,
	const std::vector<VertexAttribute>& attributes
) {
	auto it = m_vertexShaderMap.find(id);
	if (it != m_vertexShaderMap.end()) {
		return it->second;
	}

	auto vShader = GraphicsManager::GetInstance().CreateVertexShader(fileName, attributes);

	if (!vShader) {
		return nullptr;
	}

	m_vertexShaderMap.emplace(id, vShader);
	return vShader;
}

std::shared_ptr<IVertexShader> ShaderManager::GetVertexShader(const std::string& id) {
	auto it = m_vertexShaderMap.find(id);
	if (it != m_vertexShaderMap.end()) {
		return it->second;
	}

	AE::Core::Debug::LogError("Attempted to get vertex shader that does not exist, ID: " + id);

	//TODO: Return a default shader
	return nullptr;
}

std::shared_ptr<IFragmentShader> ShaderManager::CreateFragmentShader(
	const std::string& id,
	const std::string& fileName
) {
	auto it = m_fragmentShaderMap.find(id);
	if (it != m_fragmentShaderMap.end()) {
		return it->second;
	}

	auto fShader = GraphicsManager::GetInstance().CreateFragmentShader(fileName);

	if (!fShader) {
		return nullptr;
	}

	m_fragmentShaderMap.emplace(id, fShader);
	return fShader;
}

std::shared_ptr<IFragmentShader> ShaderManager::GetFragmentShader(const std::string& id) {
	auto it = m_fragmentShaderMap.find(id);
	if (it != m_fragmentShaderMap.end()) {
		return it->second;
	}

	AE::Core::Debug::LogError("Attempted to get index shader that does not exist, ID: " + id);

	//TODO: Return a default shader
	return nullptr;
}