#pragma once

#include <string>

#include "Graphics/TextureCreateInfo.h"
#include "MeshData.h"

class FileImporter {
public:
	static AE::Graphics::TextureCreateInfo ImportTexture(std::string textureName);
	static MeshData ImportMesh(std::string fileName);

	FileImporter() {/*Intentionally left blank*/ };
	~FileImporter() {/*Intentionally left blank*/ };

private:
};
