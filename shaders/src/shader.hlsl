struct Vertex
{
    float3 position : INPUT0;
    float3 color : INPUT1;
    float2 texcoord : INPUT2;
};

struct Pixel
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer CBuffer : register(b0)
{
    float4x4 mvp;
};

Pixel vs_main(Vertex input)
{
    Pixel output;
    output.position = mul(float4(input.position, 1.0), mvp);
    output.color = input.color;
    output.texcoord = input.texcoord;
    return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 ps_main(Pixel input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, input.texcoord);
    return float4(color.rgb, color.a);
}