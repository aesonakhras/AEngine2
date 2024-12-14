#pragma once
#include <memory>
#include <unordered_map>
#include <utility>
#include <string>

#include <DirectXMath.h>

#include "Core/Debug.h"
#include "Core/Common.h"

#include "Graphics/IFragmentShader.h"
#include "Graphics/IVertexShader.h"
#include "Graphics/IBuffer.h"
#include "Graphics/Texture.h"
#include "Graphics/ISampler.h"

namespace AE::Graphics {
    struct UniformDescription {
        std::string name;
        AE::Core::uint32 size;  //size of data
    };


    class Material {
    private:
        struct TextureBinding {
            std::shared_ptr<Texture> texture;
            unsigned int bindPoint;
        };

        struct SamplerBinding {
            std::shared_ptr<ISampler> sampler;
            unsigned int bindPoint;
        };
    
        public:
            Material(std::shared_ptr<IVertexShader> vertexShader,
                std::shared_ptr<IFragmentShader> fragmentShader,
                std::shared_ptr<IBuffer> ubo,
                const std::vector<UniformDescription>& uniformlayout) : m_vertexShader(vertexShader),m_fragmentShader(fragmentShader), m_ubo(ubo) {
                //uniform buffer all we need to do is specify a size?
                
                AE::Core::uint32 totalSize  = 0;

                for (const auto& uniform : uniformlayout) {
                    m_uniformDataLayout[uniform.name] = totalSize;
                    totalSize += uniform.size;
                }

                m_uniformData = new char[totalSize]();
            }

            ~Material() {
                if (m_uniformData != nullptr) {
                    delete m_uniformData;
                    m_uniformData = nullptr;
                }
            }

            template<typename T>
            void SetUniform(const std::string name, const T& value) {
                m_uniformNeedsUpdate = true;

                if (m_uniformDataLayout.contains(name)) {
                    int offset = m_uniformDataLayout[name];
                    std::memcpy(m_uniformData + offset, &value, sizeof(T));
                }
                else {
                    AE::Core::Debug::LogWarning("Uniform for material does not exist.");
                }
            }

            void SetTexture(std::string name, 
                unsigned int bindPoint, 
                std::shared_ptr<Texture> texture,
                std::shared_ptr<ISampler> sampler) {
                m_textures[name] = { texture, bindPoint };

                m_samplers[name] = { sampler, bindPoint };
            }

            void Bind() {
                m_ubo->Update(m_uniformData, m_ubo->Count);
                m_uniformNeedsUpdate = false;
                m_ubo->Bind();
                bindTextures();
                m_vertexShader->Bind();
                m_fragmentShader->Bind();
            }

            void UnBind() {
                m_ubo->UnBind();
                m_vertexShader->UnBind();
                m_fragmentShader->UnBind();
            }

        private:
            void bindTextures() {
                for (const auto& mapEntry : m_textures) {
                    //get the bind point and texture pointer pair
                    auto textureName = mapEntry.first;


                    auto texturePTR = mapEntry.second.texture;
                    auto textureBindPoint = mapEntry.second.bindPoint;

                    texturePTR->Bind(textureBindPoint);
                    m_samplers[textureName].sampler->Bind(textureBindPoint);
                }
            }
            
            std::shared_ptr<IVertexShader> m_vertexShader;
            std::shared_ptr<IFragmentShader> m_fragmentShader;
            //gpu resource
            std::shared_ptr<IBuffer> m_ubo;
            
            std::unordered_map <std::string, TextureBinding> m_textures {};
            std::unordered_map <std::string, SamplerBinding> m_samplers {};
            
            //cpu side
            std::unordered_map <std::string, AE::Core::uint32> m_uniformDataLayout;
            char* m_uniformData = nullptr;
            bool m_uniformNeedsUpdate = true;
    };
}