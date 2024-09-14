Texture2D colorTexture: register(t0);
SamplerState colorTextureSampler: register(s0);

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
    return colorTexture.Sample(colorTextureSampler, uv);
}