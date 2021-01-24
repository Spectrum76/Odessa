#include <Input.hlsli>

cbuffer Transform : register(b0)
{
    row_major matrix Model;
    row_major matrix iModel;
}

cbuffer Camera : register(b1)
{
    row_major matrix View;
    row_major matrix Proj;
    float3 CameraPosition;
}

PSInput main(VSInput Input)
{
    PSInput result;

    result.Position = mul(float4(Input.Position, 1.0f), mul(Model, mul(View, Proj)));
    result.TexCoord = Input.TexCoord;
    result.Normal = mul(Input.Normal, float3x3(iModel[0].xyz, iModel[1].xyz, iModel[2].xyz));
    result.PositionWS = mul(float4(Input.Position, 1.0f), Model).xyz;
    
    return result;
}