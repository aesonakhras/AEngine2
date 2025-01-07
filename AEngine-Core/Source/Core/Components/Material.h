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

#include "Graphics/MaterialBase.h"

namespace AE::Graphics {
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
            Material(
                std::shared_ptr<MaterialBase> materialBase,
                std::shared_ptr<IBuffer> ubo) : m_materialBase(materialBase), m_ubo(ubo) {

                m_uniformData = std::shared_ptr<char[]>(new char[materialBase->UniformSize]);
            }

            ~Material() {
  
            }

            template<typename T>
            void SetUniform(const std::string name, const T& value) {
                m_uniformNeedsUpdate = true;

                if (m_materialBase->UniformDataLayout.contains(name)) {
                    int offset = m_materialBase->UniformDataLayout[name];
                    std::memcpy(m_uniformData.get() + offset, &value, sizeof(T));
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
                m_ubo->Update(m_uniformData.get(), m_ubo->Count);
                m_uniformNeedsUpdate = false;
                m_ubo->Bind();
                bindTextures();
                m_materialBase->VertexShader->Bind();
                m_materialBase->FragmentShader->Bind();
            }

            void UnBind() {
                m_ubo->UnBind();
                m_materialBase->VertexShader->UnBind();
                m_materialBase->FragmentShader->UnBind();
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
            
            std::shared_ptr<MaterialBase> m_materialBase;
            
            //instance stuff
            //gpu resource
            std::shared_ptr<IBuffer> m_ubo;
            std::unordered_map <std::string, TextureBinding> m_textures {};
            std::unordered_map <std::string, SamplerBinding> m_samplers {};

            std::shared_ptr<char[]> m_uniformData = nullptr;
            bool m_uniformNeedsUpdate = true;
            //end instance
    };
}