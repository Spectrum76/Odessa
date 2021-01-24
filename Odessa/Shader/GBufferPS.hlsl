#include <Input.hlsli>

Texture2D BaseColor : register(t0);

SamplerState Sampler : register(s0);

PSOutput main(PSInput Input)
{
    float4 Final = BaseColor.Sample(Sampler, Input.TexCoord);
    
    if (Final.a == 0.0)
    {
        discard;
    }
    
    PSOutput Output;
    Output.Position = float4(Input.PositionWS, 0);
    Output.Normal   = float4(normalize(Input.Normal), 0);
    Output.Albedo   = Final;
    
    return Output;
}