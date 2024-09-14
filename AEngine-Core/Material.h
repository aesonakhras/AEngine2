#pragma once
#include <memory>
#include <vector>

namespace AE::Graphics {

    //forward declares
    class ISampler;
    class IShaderResourceView;
    class ITextureResource;
    class IVertexShader;
    class IFragmentShader;

    class Material {
        public:

        private:
            std::shared_ptr<ISampler> sampler;
            std::shared_ptr<IShaderResourceView> shaderResourceView;
            std::shared_ptr<ITextureResource> textureResource;
            std::shared_ptr<IVertexShader> vertexShader;
            std::shared_ptr<IFragmentShader> fragmentShader;
    };
}