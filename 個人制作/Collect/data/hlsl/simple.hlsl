Texture2D tex_diffuse : register(t0);
SamplerState sam_linear : register(s0);

cbuffer global : register(b0)
{
    row_major matrix world;
};

#include "camera.hlsli"

struct VS_Input
{
    float4 pos_ : POSITION;
    float4 normal_ : NORMAL;
    float2 uv_ : TEXCOORD;
};

struct VS_Output
{
    float4 pos_ : SV_POSITION;
    float2 uv_ : TEXCOORD;
};

VS_Output VS(VS_Input input)
{
    VS_Output output = (VS_Output) 0;
    output.pos_ = mul(float4(input.pos_.xyz, 1), world);
    output.pos_ = mul(output.pos_, view);
    output.pos_ = mul(output.pos_, projection);

    output.uv_ = input.uv_;

    return output;
}

float4 PS(VS_Output output) : SV_Target
{
    return tex_diffuse.Sample(sam_linear, output.uv_);
}