Texture2D baseColorMap: register(t0);
SamplerState baseColorSampler: register(s0);

Texture2D normalMap: register(t1);
SamplerState normalMapSampler: register(s1);

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix ModelViewProj;
    matrix Model;
    float4 viewPos;
    float4 dirLight;
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
    float3 normal : NORMAL;
    float2 uv : UV;
    
};

VOut VShader(
    VIn input
)
{
    VOut output;

    output.position = mul(float4(input.position, 1.0f), ModelViewProj);
    output.normal = normalize(mul(Model, float4(input.normal,0)));
    output.uv = input.uv;

    return output;
}


float4 PShader(
    VOut input
) : SV_TARGET
{
    float4 dirColor = float4(1, 1, 1, 1);

    float4 baseColor = baseColorMap.Sample(baseColorSampler, input.uv);
    float4 color2 = normalMap.Sample(normalMapSampler, input.uv);

    float4 ambient = 0.05f * baseColor;

    float diffuseIntensity = max(dot(input.normal, -dirLight), 0.0f);
    float4 diffuse = diffuseIntensity * dirColor * baseColor;
    

    return ambient + diffuse;
}