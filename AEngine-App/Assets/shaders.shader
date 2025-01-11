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
    float3 TtoW0 : CUSTOM0;
    float3 TtoW1 : CUSTOM1;
    float3 TtoW2 : CUSTOM2;
};

VOut VShader(
    VIn input
)
{
    VOut output;

    output.position = mul(float4(input.position, 1.0f), ModelViewProj);
    output.normal = normalize(mul(Model, float4(input.normal,0)));
    output.uv = input.uv;

    // Build Tangent-to-World matrix rows
    float3x3 TBN = float3x3(
        normalize(mul(Model, float4(input.tangent, 0)).xyz),
        normalize(mul(Model, float4(input.bitangent, 0)).xyz),
        normalize(mul(Model, float4(input.normal, 0)).xyz)
    );

    output.TtoW0 = TBN[0];
    output.TtoW1 = TBN[1];
    output.TtoW2 = TBN[2];

    return output;
}


float4 PShader(
    VOut input
) : SV_TARGET
{
    float4 dirColor = float4(1, 1, 1, 1);

    float4 baseColor = baseColorMap.Sample(baseColorSampler, input.uv);
    float4 normal = normalMap.Sample(normalMapSampler, input.uv);

    //extract normal
    normal = normalize(normal * 2.0f - 1.0f);

    float3 normalWorld = normalize(
        normal.x * input.TtoW0 +
        normal.y * input.TtoW1 +
        normal.z * input.TtoW2
    );

    float4 ambient = 0.05f * baseColor;

    float diffuseIntensity = max(dot(normalWorld, -dirLight.xyz), 0.0f);
    float4 diffuse = diffuseIntensity * dirColor * baseColor;

    float4 color = ambient + diffuse;


    return color;
}