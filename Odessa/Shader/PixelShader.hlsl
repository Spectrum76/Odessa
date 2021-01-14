#include <Input.hlsli>

Texture2D BaseColor : register(t0);

SamplerState Sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    return BaseColor.Sample(Sampler, input.TexCoord);
}