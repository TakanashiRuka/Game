Texture2D tex_diffuse : register(t0);
Texture2D tex_depth : register(t4);
SamplerState sam_linear : register(s0);
SamplerComparisonState sam_shadow : register(s1);

cbuffer global : register(b0)
{
	row_major matrix world;
	float4 texel_size;
	float4 out_line_color;
	float border_width;
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
	float4 normal_ : NORMAL;
	float2 uv_ : TEXCOORD0;
	float4 pos_lwvp_ : POSITION0;
	float4 gs_pos_ : POSITION1;
	bool out_line_ : BOOL;
};

VS_Output VS(VS_Input input)
{
	VS_Output output = (VS_Output) 0;

	output.pos_ = mul(float4(input.pos_.xyz, 1.0f), world);
	output.pos_ = mul(output.pos_, view);
	output.pos_ = mul(output.pos_, projection);

	output.pos_lwvp_ = mul(float4(input.pos_.xyz, 1.0f), world);
	output.pos_lwvp_ = mul(output.pos_lwvp_, light_view);
	output.pos_lwvp_ = mul(output.pos_lwvp_, light_projection);

	input.normal_.w = 0;
	output.normal_ = normalize(mul(input.normal_, world));

	output.uv_ = input.uv_;

	output.gs_pos_ = mul(float4(input.pos_.xyz, 1.0f) + input.normal_ * border_width, world);
	output.gs_pos_ = mul(output.gs_pos_, view);
	output.gs_pos_ = mul(output.gs_pos_, projection);

	return output;
}

[maxvertexcount(6)]
void GS(triangle VS_Output triangle_output[3], inout TriangleStream<VS_Output> triangle_stream)
{
	VS_Output output = (VS_Output) 0;
 
	int v;
	for (v = 0; v < 3; ++v)
	{
		output.pos_ = triangle_output[v].pos_;
		output.normal_ = triangle_output[v].normal_;
		output.uv_ = triangle_output[v].uv_;
		output.pos_lwvp_ = triangle_output[v].pos_lwvp_;
		output.out_line_ = false;

		triangle_stream.Append(output);
	}

	triangle_stream.RestartStrip();

	for (v = 2; v >= 0; --v)
	{
		output.pos_ = triangle_output[v].gs_pos_;
		output.normal_ = triangle_output[v].normal_;
		output.uv_ = triangle_output[v].uv_;
		output.pos_lwvp_ = triangle_output[v].pos_lwvp_;
		output.out_line_ = true;

		triangle_stream.Append(output);
	}

	triangle_stream.RestartStrip();
}

float4 PS(VS_Output output) : SV_Target
{
	float4 pos_lwvp = output.pos_lwvp_;
	float2 texel_l = float2(pos_lwvp.x / pos_lwvp.w * 0.5f + 0.5f, pos_lwvp.y / pos_lwvp.w * -0.5f + 0.5f);

	float4 color = 0.0f;
	float shadow_color = 0.0f;

	if (texel_l.x < 0 || texel_l.x > 1 || texel_l.y < 0 || texel_l.y > 1)
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
					texel_l.x + (float) i * texel_size.x,
					texel_l.y + (float) j * texel_size.y),
					depthcompare);
				++cnt;
			}
		}

		percent_lit = percent_lit / cnt;
		shadow_color = clamp(percent_lit, 0.3f, 1);
	}

	if (!output.out_line_)
	{
		float4 nl = normalize(light);
		float lambert = dot(nl.xyz, output.normal_.xyz);
		lambert = lambert * 0.5f + 0.5f;

		float4 decalmap = tex_diffuse.Sample(sam_linear, output.uv_);
		color.rgb = (decalmap * min(lambert, shadow_color)).rgb;
		color.a = tex_diffuse.Sample(sam_linear, output.uv_).a;

	}
	else
	{
		color = out_line_color.rgba;
	}

	return color;
}