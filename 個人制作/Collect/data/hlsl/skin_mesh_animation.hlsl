Texture2D tex_diffuse : register(t0);
SamplerState sam_linear : register(s0);

#define BONE_MAX 128

cbuffer SkinMeshAnimation : register(b0)
{
    row_major matrix world;
    row_major matrix bones[BONE_MAX];
};

#include "light.hlsli"

#include "camera.hlsli"

struct Skin
{
    float4 pos_;
    float4 normal_;
};

struct VS_Input
{
    float4 pos_ : POSITION;
    float4 normal_ : NORMAL;
    float2 uv_ : TEXCOORD;
    uint4 index_ : INDEX;
    float4 weight_ : WEIGHT;
};

struct VS_Output
{
    float4 pos_ : SV_POSITION;
    float4 normal_ : NORMAL;
    float2 uv_ : TEXCOORD;
};

VS_Output VS(VS_Input input)
{
    VS_Output output = (VS_Output) 0;
    Skin skin = (Skin) 0;

    float4 pos = float4(input.pos_.xyz, 1);
    input.normal_.w = 0;
    row_major matrix bone;
    float weight = 0;

    bone = bones[input.index_.x];
    weight = input.weight_.x;
    skin.pos_ += weight * mul(pos, bone);
    skin.normal_ += weight * mul(input.normal_, bone);

    bone = bones[input.index_.y];
    weight = input.weight_.y;
    skin.pos_ += weight * mul(pos, bone);
    skin.normal_ += weight * mul(input.normal_, bone);

    bone = bones[input.index_.z];
    weight = input.weight_.z;
    skin.pos_ += weight * mul(pos, bone);
    skin.normal_ += weight * mul(input.normal_, bone);

    bone = bones[input.index_.w];
    weight = input.weight_.w;
    skin.pos_ += weight * mul(pos, bone);
    skin.normal_ += weight * mul(input.normal_, bone);

    output.pos_ = mul(skin.pos_, world);
    output.pos_ = mul(output.pos_, view);
    output.pos_ = mul(output.pos_, projection);

    output.normal_ = normalize(mul(skin.normal_, world));

    output.uv_ = input.uv_;

    return output;
}

float4 PS(VS_Output output) : SV_Target
{
    float4 color = 0;
    float shadow_color = 0;

    shadow_color = 1;

    float lambert = dot(normalize(light).xyz, output.normal_.xyz);
    lambert = lambert * 0.5f + 0.5f;

    float4 decalmap = tex_diffuse.Sample(sam_linear, output.uv_);
    color = (decalmap * min(lambert, shadow_color)).rgba;

    return color;
}