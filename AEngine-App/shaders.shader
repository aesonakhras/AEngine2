Texture2D colorTexture: register(t0);
SamplerState colorTextureSampler: register(s0);

Texture2D colorTexture2: register(t1);
SamplerState colorTextureSampler2: register(s1);

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix mWorldViewProj;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

VOut VShader(float3 position : POSITION, float2 uv : UV)
{
    VOut output;

    output.position = mul(float4(position, 1.0f), mWorldViewProj);
    output.uv = uv;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float2 uv : UV) : SV_TARGET
{
    float4 color1 = colorTexture.Sample(colorTextureSampler, uv);
    float4 color2 = colorTexture2.Sample(colorTextureSampler2, uv);


    float step = smoothstep(0.2, 0.8, uv.x);
    return lerp(color1, color2, step);

    //return colorTexture.Sample(colorTextureSampler, uv);
}