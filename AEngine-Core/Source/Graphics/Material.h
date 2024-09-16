#pragma once
#include <memory>
#include <unordered_map>
#include <utility>
#include <string>

#include <DirectXMath.h>

#include "Core/Debug.h"

#include "IFragmentShader.h"
#include "IVertexShader.h"
#include "IBuffer.h"
#include "Texture.h"
#include "ISampler.h"

namespace AE::Graphics {
    class Material {
        public:
            Material(std::shared_ptr<IVertexShader> vertexShader,
                std::shared_ptr<IFragmentShader> fragmentShader,
                std::shared_ptr<IBuffer> ubo) : m_vertexShader(vertexShader),m_fragmentShader(fragmentShader), m_ubo(ubo) {
                //uniform buffer all we need to do is specify a size?
            }

            void SetUBO(void* data, size_t size) {
                assert(size == m_ubo->Count && "Trying to adjust UBO with incorrectly sized data");

                m_ubo->Update(data, size);
            }

            void SetTexture(std::string name, unsigned int bindPoint, std::shared_ptr<Texture> texture, std::shared_ptr<ISampler> sampler) {
                m_textures[name] = std::pair<std::shared_ptr<Texture>, unsigned int>(texture, bindPoint);
                m_samplers[name] = std::pair<std::shared_ptr<ISampler>, unsigned int>(sampler, bindPoint);
            }

            void Bind() {
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


                    auto texturePTR = mapEntry.second.first;
                    auto textureBindPoint = mapEntry.second.second;

                    texturePTR->Bind(textureBindPoint);
                    m_samplers[textureName].first->Bind(textureBindPoint);
                }
            }
            
            std::shared_ptr<IVertexShader> m_vertexShader;
            std::shared_ptr<IFragmentShader> m_fragmentShader;
            std::shared_ptr<IBuffer> m_ubo;
            
            std::unordered_map <std::string, std::pair<std::shared_ptr<Texture>, unsigned int>> m_textures {};
            std::unordered_map <std::string, std::pair<std::shared_ptr<ISampler>, unsigned int>> m_samplers {};
    };
}