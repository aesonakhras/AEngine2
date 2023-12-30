//adapted from https://gamedev.stackexchange.com/questions/98283/how-do-i-draw-a-full-screen-quad-in-directx-11
Texture2D colorTexture: register(t0);
SamplerState colorTextureSampler: register(s0);

struct VOut
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

VOut VShader(uint vI : SV_VERTEXID)
{
    VOut output;

    float2 texcoord = float2(vI & 1,vI >> 1); //you can use these for texture coordinates later
    output.position =  float4((texcoord.x - 0.5f) * 2,-(texcoord.y - 0.5f) * 2,0,1);
    output.uv = texcoord;

    return output;
}

float4 PShader(float4 pos : SV_POSITION, float2 uv : UV) : SV_TARGET
{
    //return float4(1,0,0,1); //the red color
    return colorTexture.Sample(colorTextureSampler, uv);
}