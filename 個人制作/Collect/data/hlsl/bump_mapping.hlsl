Texture2D tex_diffuse : register(t0);
Texture2D tex_normal : register(t1);
SamplerState sam_linear : register(s0);

cbuffer global : register(b0)
{
    row_major matrix world;
    row_major matrix wit;
};

#include "light.hlsli"

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
    float3 normal_ : NORMAL;
    float3 light_ : LIGHT;
    float3 half_vector_ : TEXCOORD0;
    float3 eye_vector_ : TEXCOORD1;
    float2 uv_ : TEXCOORD2;
};

VS_Output VS(VS_Input input)
{
    float3 world_pos = mul(input.pos_, world).xyz;
    float3 light_vector = normalize(light).xyz;
    float3 eye_vector = normalize(eye.xyz - world_pos);
    float3 harf_vector = normalize(light_vector + eye_vector);

    float3 tangent3 = cross(input.normal_.xyz, float3(0, 1, 0));
    float3 t = mul(tangent3, (float3x3) wit);
    float3 n = mul(input.normal_.xyz, (float3x3) wit);
    float3 b = cross(n, t);
    float3x3 tangent3x3 = float3x3(
        t.x, b.x, n.x,
        t.y, b.y, n.y,
        t.z, b.z, n.z);

    VS_Output output = (VS_Output) 0;
    output.pos_ = mul(float4(input.pos_.xyz, 1), world);
    output.pos_ = mul(output.pos_, view);
    output.pos_ = mul(output.pos_, projection);
    input.normal_.w = 0;
    output.normal_ = normalize(mul(input.normal_, world)).xyz;
    output.uv_ = input.uv_;
    output.half_vector_ = mul(harf_vector, tangent3x3);
    output.light_ = mul(light_vector, tangent3x3);

    return output;
}

float4 PS(VS_Output output) : SV_Target
{
    float3 o_half_vector = normalize(output.half_vector_);
    float3 o_light = normalize(output.light_);
    float3 o_normal = normalize(tex_normal.Sample(sam_linear, output.uv_).rgb * 2.0f - 1.0f);
    float nh = saturate(dot(o_normal, o_half_vector));
    float nl = saturate(dot(o_normal, o_light));
    float power = (nl == 0.0f) ? 0.0f : pow(nh, 4);
    
    float4 color = ambient + (diffuse * nl) + (specular * power);
    color = color * tex_diffuse.Sample(sam_linear, output.uv_);

    return color;
}