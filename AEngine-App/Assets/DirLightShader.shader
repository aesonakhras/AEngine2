struct VIn
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 uv : UV;
};

struct VOut
{
    float4 posLightSpace : SV_POSITION;
};

cbuffer LightMatrices : register(b0)
{
    matrix lightViewProj;
}

VOut VShader(VIn input)
{
    VOut output;
    output.posLightSpace = mul(lightViewProj, float4(input.position, 1.0f));
    return output;
}

void PShader(VOut input)
{
    //depth only
}