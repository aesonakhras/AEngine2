#include "Assets/LightBuffer.shader"

static const float PI = 3.14159265f;

Texture2D baseColorMap: register(t0);
SamplerState baseColorSampler: register(s0);

Texture2D normalMap: register(t1);
SamplerState normalMapSampler: register(s1);

Texture2D metallicMap: register(t2);
SamplerState metallicSampler: register(s2);

Texture2D roughnessMap: register(t3);
SamplerState roughnessSampler: register(s3);

Texture2D brdf_lut: register(t4);
SamplerState brdf_lutSampler: register(s4);

TextureCube envRadiance : register(t8); 
SamplerState envRadianceSampler : register(s8);

TextureCube envIrradiance : register(t9); 
SamplerState envIrradianceSampler : register(s9);

Texture2D dirLightDepthMap: register(t10);
SamplerState dirLightSampler: register(s10);

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix ModelViewProj;
    matrix Model;
    matrix LightSpaceMatrix;
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
    float3 fragPos : TEXCOORD0;
    float4 FragPosLightSpace : TEXCOORD1;
    float3 V : TEXCOORD2;
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

    output.position = mul(ModelViewProj, float4(input.position, 1.0f));
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

    float4 worldPosition = mul(Model, float4(input.position, 1.0));
    output.fragPos = worldPosition.xyz;

    worldPosition.w = 1.0;

    output.FragPosLightSpace = mul(LightSpaceMatrix, worldPosition);
    
    output.V = viewPos.xyz;
    return output;
}

float3 FresnelSchlickRoughness(float VdotH, float3 F0, float roughness) {
    float oneMinusRough = 1.0 - roughness;

    float3 F0_mod = max(float3(oneMinusRough, oneMinusRough, oneMinusRough), F0);
    return F0 + (F0_mod - F0) * exp2((-5.55473 * VdotH - 6.98316) * VdotH);
}

float3 FresnelSchlick(float cosTheta, float3 F0) {
    return F0 + (1.0 - F0) * exp2((-5.55473 * cosTheta - 6.98316) * cosTheta);
}

float3 fresnelSchlick2(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0001, 1.0), 5.0);
}

//D(x) Trowbridge-Reitz
float DistributionGGX(float3 N, float3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

//F(x)
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

//G(X)
float GeometrySmith(float3 N, float3 V, float3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

float3 SpecularBRDF(
    float3 L,
    float3 N,
    float3 V,
    float3 diffColor,
    float roughness,
    float metallic
) {
    float3 H = normalize(V+L);
    float VdotH = max(dot(H, V), 0.0);

    float3 F0 = float3(0.04f, 0.04f, 0.04f); 
    F0 = lerp(F0, diffColor.rgb, metallic);

    float D = DistributionGGX(N, H, roughness);
    float3 F = FresnelSchlick(VdotH, F0);
    float G = GeometrySmith(N, V, L, roughness);

    float NdotL = max(dot(N, L), 0.0001);
    float NdotV = max(dot(N, V), 0.0001);
    float3 specularBRDF = (D * F * G) / (4.0 * NdotL * NdotV + 0.0001);

    float3 diffuseBRDF = diffColor / PI;

    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - metallic);

    return (kD * diffuseBRDF + specularBRDF) * NdotL;
}

float4 CalculatePointLight(
    float3 lightPosition,
    float3 lightColor,
    float3 fragPos,
    float3 N,
    float3 V,
    float3 diffColor,
    float roughness,
    float metallic
) {
    // Direction from the fragment to the light source
    float3 L = lightPosition - fragPos;
    float distance = length(L);
    L = normalize(L);

    float3 specular = SpecularBRDF (
        L,
        N,
        V,
        diffColor,
        roughness,
        metallic
    );

    float NdotL = max(dot(N, L), 0.0001);

    float attenuation = 1.0 / (1.0 + 0.09f * distance + 0.032f * distance * distance);

    float3 finalColor = lightColor * attenuation * 10 * specular * NdotL;

    return float4(finalColor, 1.0);
}

float4 calculateDirLight(
    float3 lightDir,
    float3 lightColor,
    float3 fragPos,
    float3 N,
    float3 V,
    float3 diffColor,
    float roughness,
    float metallic) {

    float3 specular = SpecularBRDF (
        lightDir,
        N,
        V,
        diffColor,
        roughness,
        metallic
    );

    float NdotL = max(dot(N, lightDir), 0.0001);

    float3 adjustedLightColor = lightColor; 
    return float4(adjustedLightColor * NdotL * specular, 1);

}

float calculateShadows(float4 fragPosLightSpace, float2 uv, float3 N, float3 lightDir) {
    float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //float bias = 0.0000001;
    float bias = max(0.00005f * (1.0f - dot(N, lightDir)), 0.000001f);
    float shadow = 0.0;

    //float2 shadowUV = fragPosLightSpace.xy / fragPosLightSpace.w;
    float2 shadowUV = fragPosLightSpace.xy / fragPosLightSpace.w * 0.5 + 0.5;
    float shadowDepth = fragPosLightSpace.z / fragPosLightSpace.w;

    float actualX = shadowUV.x;
    float actualY = 1 -  shadowUV.y;

    float2 texelSize = 1.0 / float2(2048, 2048);

    for(int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float2 offset = float2(x, y) * texelSize;
            float closestDepth = dirLightDepthMap.Sample(dirLightSampler, float2(actualX, actualY) + offset).r;
            shadow += shadowDepth > closestDepth + bias ? 0.0f : 1.0f;
        }
    }

    return shadow / 9;
}

float4 PShader(
    VOut input
) : SV_TARGET
{
    float3 V = normalize(input.V - input.fragPos);


    float4 dirColor = float4(1, 1, 1, 1);
    input.uv = input.uv * 1;
    float3 albedo = baseColorMap.Sample(baseColorSampler, input.uv);
    float roughness = roughnessMap.Sample(roughnessSampler, input.uv).r;
    //roughness = 0.0f;
    float metallic = metallicMap.Sample(metallicSampler, input.uv).r;
    float3 normal = normalMap.Sample(normalMapSampler, input.uv).xyz;

    float3 f0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    //extract normal
    normal = normalize(normal * 2.0f - 1.0f);

    float3 N = normalize(
        normal.x * input.TtoW0 +
        normal.y * input.TtoW1 +
        normal.z * input.TtoW2
    );

    const float REFLECTIONLOD = 6.0f;
    float3 R = reflect(-V, N);

    float4 color = float4(0,0,0,0);

    float shadowFactor = calculateShadows(input.FragPosLightSpace, input.uv, N, normalize(dirLight.xyz));

    // Calculate point light contribution
    for (int i = 0; i < NumLights; i++) {
        color += CalculatePointLight(
            lights[i].position,
            lights[i].color,
            input.fragPos,
            N,
            V,
            albedo,
            roughness,
            metallic
        );
    }

    //dirlight calculation
    color += calculateDirLight(
            normalize(dirLight.xyz),
            float3(1,1,1),
            input.fragPos,
            N,
            V,
            albedo,
            roughness,
            metallic
        ) * shadowFactor;

    float NdotV = max(dot(N, V), 0.0001);

    float3 F0 = float3(0.04f, 0.04f, 0.04f); 
    F0 = lerp(F0, albedo.rgb, metallic);

    float3 F =  FresnelSchlickRoughness(NdotV, F0, roughness);

    float3 kS = F;
    float3 kD = 1.0 - kS;

    float3 irradiance = envIrradiance.Sample(envIrradianceSampler, N);
    float3 diffuse = irradiance * albedo;
    diffuse *= shadowFactor * 0.5 + 0.5;


    float3 prefilteredColor = envRadiance.SampleLevel(envRadianceSampler, R, roughness * REFLECTIONLOD);
    float2 brdf = brdf_lut.Sample(brdf_lutSampler, float2(NdotV, roughness)).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y) * shadowFactor;

    float3 ambient = (kD * diffuse + specular) ;

    color += float4(ambient, 1);

    return color;
    //return float4(R, 1);

}