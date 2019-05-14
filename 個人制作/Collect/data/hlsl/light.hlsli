cbuffer Light : register(b2)
{
    row_major matrix light_view;
    row_major matrix light_projection;

    float4 light;
    float4 ambient;
    float4 diffuse;
    float4 specular;
};