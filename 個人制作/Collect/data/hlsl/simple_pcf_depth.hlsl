cbuffer global : register(b0)
{
    row_major matrix world;
    float4 texel_size;
};

#include "light.hlsli"

#include "camera.hlsli"

struct VS_Input
{
    float3 pos_ : POSITION;
    float3 normal_ : NORMAL;
    float2 uv_ : TEXCOORD;
};

struct VS_Output
{
    float4 pos_ : SV_POSITION;
    float4 depth_ : POSITION;
};

VS_Output VS(VS_Input input)
{
    VS_Output output = (VS_Output) 0;

    output.pos_ = mul(float4(input.pos_.xyz, 1.0f), world);
    output.pos_ = mul(output.pos_, light_view);
    output.pos_ = mul(output.pos_, light_projection);

    output.depth_ = output.pos_;

    return output;
}

float4 PS(VS_Output output) : SV_Target
{
    float4 color = output.depth_.z / output.depth_.w;

    return color;
}