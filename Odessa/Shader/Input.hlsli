struct PSInput
{
    float4 Position     : SV_POSITION;
    float2 TexCoord     : TEXCOORD;
    float3 Normal       : NORMAL;
    float3 PositionWS   : POSITIONWS;
};

struct VSInput
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal   : NORMAL;
};

struct PSOutput
{
    float4 Position : SV_Target0;
    float4 Normal   : SV_Target1;
    float4 Albedo   : SV_Target2;
};