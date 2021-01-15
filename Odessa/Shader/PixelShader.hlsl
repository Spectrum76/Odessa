#include <Input.hlsli>

Texture2D BaseColor : register(t0);

SamplerState Sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float4 Final = BaseColor.Sample(Sampler, input.TexCoord);
    
    if (Final.a == 0.0)
    {
        discard;
    }
    
    return Final;
}