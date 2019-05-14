Texture2D tex_diffuse : register(t0);
Texture2D tex_depth : register(t4);
SamplerState sam_linear : register(s0);
SamplerComparisonState sam_shadow : register(s1);

#define BONE_MAX 128

cbuffer SkinMeshAnimation : register(b0)
{
	row_major matrix world;
	row_major matrix bones[BONE_MAX];
	float4 texel_size;
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
	float2 uv_ : TEXCOORD0;
	float4 pos_lwvp_ : POSITION0;
	float4 gs_pos_ : POSITION1;
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

	output.pos_lwvp_ = mul(skin.pos_, world);
	output.pos_lwvp_ = mul(output.pos_lwvp_, light_view);
	output.pos_lwvp_ = mul(output.pos_lwvp_, light_projection);

	output.normal_ = normalize(mul(skin.normal_, world));

	output.uv_ = input.uv_;

	return output;
}

float4 PS(VS_Output output) : SV_Target
{
	float4 pos_lwvp = output.pos_lwvp_;
	float2 texel = float2(pos_lwvp.x / pos_lwvp.w * 0.5f + 0.5f, pos_lwvp.y / pos_lwvp.w * -0.5f + 0.5f);

	float4 color = 0.0f;
	float shadow_color = 0.0f;
	if (texel.x < 0 || texel.x > 1 || texel.y < 0 || texel.y > 1)
	{
		shadow_color = 1;
		//discard;
	}
	else
	{
		float depthcompare = pos_lwvp.z / pos_lwvp.w - 0.005f;
		
		float percent_lit = 0;
		float cnt = 0;

		for (int i = -4; i < 5; i += 2)
		{
			for (int j = -4; j < 5; j += 2)
			{
				percent_lit += tex_depth.SampleCmpLevelZero(sam_shadow,
				float2(
				texel.x + (float) i * texel_size.x,
				texel.y + (float) j * texel_size.y),
				depthcompare);
				++cnt;
			}
		}

		percent_lit = percent_lit / cnt;
		shadow_color = clamp(percent_lit, 0.3f, 1);
	}

	float4 nl = normalize(light);
	float lambert = dot(nl.xyz, output.normal_.xyz);
	lambert = lambert * 0.5f + 0.5f;

	float4 decalmap = tex_diffuse.Sample(sam_linear, output.uv_);
	color.rgb = (decalmap * min(lambert, shadow_color)).rgb;
	color.a = tex_diffuse.Sample(sam_linear, output.uv_).a;

	return color;
}