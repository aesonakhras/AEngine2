#pragma once
#include <vector>

namespace AE::Graphics {
    //forward declare
    struct VertexAttribute;

    class IVertexDescription {
        virtual const std::vector<VertexAttribute>& Get() = 0;
    };
}