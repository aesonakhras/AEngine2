#pragma once

#include <string>

#include "Graphics/TextureCreateInfo.h"
#include "MeshData.h"

class FileImporter {
public:
	static AE::Graphics::TextureCreateInfo ImportTexture(std::string textureName, bool isCubeMap);
	static MeshData ImportMesh(std::string fileName);

private:
	static void ImportTexture2D(const std::string& fileName, AE::Graphics::TextureCreateInfo& textureCreateInfo);
	static void ImportCubeMap(const std::string& fileName, AE::Graphics::TextureCreateInfo& textureCreateInfo);
	static unsigned char* ImportImageFile(const std::string& fileName, uint32& width, uint32& height, uint32& channels);


	//verty verbose since I do not plan to have this here for that long
	//TODO: find another solution for generating and loading this file
	static void ImportBinTextureBRDFLUT128x128R16G16(const std::string& fileName, AE::Graphics::TextureCreateInfo& textureCreateInfo);

	static AE::Graphics::TextureFormat DetermineTextureFormatFromChannels(uint32 channels);

	static void ImportKTXFile(const std::string& filename, AE::Graphics::TextureCreateInfo& textureCreateInfo);
	FileImporter() {/*Intentionally left blank*/ };
	~FileImporter() {/*Intentionally left blank*/ };

private:
};
