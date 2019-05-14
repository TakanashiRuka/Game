cbuffer Camera : register(b3)
{
    row_major matrix view;
    row_major matrix projection;
    row_major matrix view_inverse;

    float4 eye;
};