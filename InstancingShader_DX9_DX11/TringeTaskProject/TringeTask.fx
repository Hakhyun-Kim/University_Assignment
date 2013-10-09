//
// File: TringeTask.fx
//

texture g_txScene : TEXTURE;
float4x4 g_mWorld : WORLD : register(c0);
float4x4 g_mView : VIEW : register(c4);
float4x4 g_mProj : PROJECTION : register(c8);

float4 g_vLightPositionView : LIGHTPOSITIONVIEW;

#define g_nNumBatchInstance 48
float4 g_vBoxInstance_Position[g_nNumBatchInstance] : BOXINSTANCEARRAY_POSITION : register(c16);

float g_specLevel = 0.5;
float specExpon=5.0;

//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------
sampler g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

void VS_ShaderInstancing( float4 vPos : POSITION,
						float3 vNormal : NORMAL,
						float2 vTex0 : TEXCOORD0,
						float vBoxInstanceIndex : TEXCOORD1,
						out float4 oPos : POSITION,
						out float2 oTex0 : TEXCOORD0,
						out float3 oTex1 : TEXCOORD1,
						out float3 oTex2 : TEXCOORD2
						 )
{
	float4 vBoxInstance = g_vBoxInstance_Position[vBoxInstanceIndex];
		
	vPos += float4( vBoxInstance.xyz * 32 - 16, 0 );
	
	oPos = mul( vPos, g_mWorld );
	oPos = mul( oPos, g_mView );
	oPos = mul( oPos, g_mProj );
	
	oTex0 = vTex0;

	float4 vViewPosition = mul( float4(oPos.xyz, 1.0f), g_mView);
    float3 vViewNormal = normalize(mul(vNormal, (float3x3)g_mView));
	oTex1 = vViewPosition.xyz;
    oTex2 = vViewNormal;
}


float4 PointLight
    (
    in float2 vTexture : TEXCOORD0,
    in float3 vViewPosition : TEXCOORD1,
    in float3 vNormal : TEXCOORD2
    ) : COLOR
{
    float3 vPointToCamera = normalize(-vViewPosition);
    float4 output;

    float3 vIntensity = float3(0.02f, 0.02f, 0.02f);
        
	float3 vLightToPoint = normalize(vViewPosition - g_vLightPositionView );
	float3 vReflection   = reflect(vLightToPoint, vNormal);
	float  fPhongValue   = saturate(dot(vReflection, vPointToCamera));

	float  fDiffuse      = saturate(dot(vNormal, -vLightToPoint)) * 10000;
	float  fSpecular     = g_specLevel * pow(fPhongValue, specExpon);

	float fDistance = distance(g_vLightPositionView , vViewPosition);
	vIntensity += (fDiffuse+fSpecular) / (fDistance*fDistance);
	
	output = tex2D(g_samScene, vTexture);
	output.xyz *= vIntensity;

    return output;
}

float4 PS(	float2 vTex0 : TEXCOORD0 ) : COLOR0
{
    return tex2D( g_samScene, vTex0 );
}

technique TShader_TringeTask
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_ShaderInstancing();
        PixelShader  = compile ps_2_0 PointLight();
        ZEnable = true;
        AlphaBlendEnable = true;
    }
}
