Texture2D colorTexture: register(t0);
SamplerState colorTextureSampler: register(s0);

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix mWorldViewProj;
    float4 cb_color;
};


struct VOut
{
    float4 position : SV_POSITION;
    float4 uv : UV;
};

VOut VShader(float3 position : POSITION, float4 uv : UV)
{
    VOut output;

    output.position = mul(float4(position, 1.0f), mWorldViewProj);

    //output.position = position;
    output.uv = uv;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 uv : UV) : SV_TARGET
{
    //return color;
    //return float4(uv.x, uv.y, 0, 1);

    return colorTexture.Sample(colorTextureSampler, uv);
}