TextureCube skyboxTexture : register(t0); 
SamplerState samplerState : register(s0);

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix vpNoPosition;
};

struct VIn {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 uv : UV;
};

struct VOut {
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;    
};

VOut VShader(
    VIn input
)
{
    VOut output;

    output.position = mul(float4(input.position, 1.0f), vpNoPosition);
    output.position.z = output.position.w;
    output.texCoord = input.position;

    return output;
}


float4 PShader(
    VOut input
) : SV_TARGET
{
    // Normalize the direction vector for cubemap sampling
    float3 direction = normalize(input.texCoord);

    // Sample the cubemap texture
    return skyboxTexture.Sample(samplerState, direction);
}