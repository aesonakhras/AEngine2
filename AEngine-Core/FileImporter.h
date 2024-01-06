#pragma once
#include "MeshData.h"
#include <string>
#include "TextureData.h"

class FileImporter {
public:
	static TextureData ImportTexture(std::string textureName);
	static MeshData ImportMesh(std::string fileName);

	

	FileImporter() {/*Intentionally left blank*/ };
	~FileImporter() {/*Intentionally left blank*/ };

private:
};
