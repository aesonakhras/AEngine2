#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "IFragmentShader.h"
#include "IVertexShader.h"
#include "IBuffer.h"

namespace AE::Graphics {
    class Material {
        public:
            Material(std::shared_ptr<IVertexShader> vertexShader,
                std::shared_ptr<IFragmentShader> fragmentShader,
                std::shared_ptr<IBuffer> ubo) : m_vertexShader(vertexShader),m_fragmentShader(fragmentShader), m_ubo(ubo) {
                //uniform buffer all we need to do is specify a size?
            }

            void SetMVP(void* data, size_t size) {
                m_ubo->Update(data, size);
            }

            void Bind() {
                m_ubo->Bind();
                m_vertexShader->Bind();
                m_fragmentShader->Bind();
            }

            void UnBind() {
                m_ubo->UnBind();
                m_vertexShader->UnBind();
                m_fragmentShader->UnBind();
            }

        private:
            //std::vector<Texture>
            std::shared_ptr<IVertexShader> m_vertexShader;
            std::shared_ptr<IFragmentShader> m_fragmentShader;
            std::shared_ptr<IBuffer> m_ubo;
            //multiple textures

    };
}