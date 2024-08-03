#pragma once
#include <memory>
#include <vector>
#include <../IBuffer.h>
#include <../IShader.h>
#include <../IInputLayout.h>

namespace AEngine {
    namespace Graphics {

        class Material {
        public:
            Material(const std::shared_ptr<IShader>& vs,
                const std::shared_ptr<IShader>& ps,
                const std::shared_ptr<ILayout>& layout);
            ~Material();

            void SetVertexShader(const std::shared_ptr<IShader>& vs);
            void SetPixelShader(const std::shared_ptr<IShader>& ps);
            void SetConstantBuffer(uint32_t slot, const std::shared_ptr<IBuffer>& cb);

            template <typename T>
            void SetInputLayout(const std::shared_ptr<ILayout>& layout) { m_InputLayout = layout; }
            void SetTexture(uint32_t slot, const std::shared_ptr<void*>& texture);

            void Bind() const;
            void UnBind() const;

        private:
            std::shared_ptr<IShader> m_VertexShader;
            std::shared_ptr<IShader> m_PixelShader;
            std::vector<std::shared_ptr<IBuffer>> m_ConstantBuffers;
            std::shared_ptr<ILayout> m_InputLayout;

            std::vector<std::shared_ptr<void*>> m_Textures;

            // Other resources like input layout, sampler states, etc.
        };

    }
}