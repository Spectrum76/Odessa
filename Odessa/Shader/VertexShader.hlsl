#include <Input.hlsli>

PSInput main(float3 Position : POSITION, float2 TexCoord : TEXCOORD, float3 Normal : NORMAL)
{
    PSInput result;

    result.Position = float4(Position, 1.0f);
    result.TexCoord = TexCoord;
    result.Normal = Normal;
    
    return result;
}