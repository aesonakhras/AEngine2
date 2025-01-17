#ifndef LIGHTBUFFER_HLSL
#define LIGHTBUFFER_HLSL

cbuffer LightBuffer : register(b3) {
    struct Light {
        float4 position;
        float4 color;
        float radius;
        float linearAttenuation;
        float quadraticAttenuation;
        float padding;
    };

    Light lights[128];
    int lightCount;
    int padding[3];
};

#endif // LIGHTBUFFER_HLSL