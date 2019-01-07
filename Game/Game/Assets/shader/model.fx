cbuffer cb : register(b0)
{
	float4x4 mvp;	//ワールドビュープロジェクション行列
	float4x4 viewProj;
	float specularPower;
};

cbuffer lightCB : register(b1)
{
	float4 ambientLight;
	float4 diffuseLight[4];
	float4 diffuseLightDir[4];
};

cbuffer shadowCB : register(b2)
{
	float4x4 lightViewProj;
};

cbuffer materialCB : register(b3)
{
	int isShadowReceiver;
	int isNormalMap;
	int isDiffuse;
	int isSpecularMap;
};

StructuredBuffer<float4x4> boneMatrix : register(t100);

struct VS_INPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD0;
};

struct VS_SKIN_INPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD0;
	uint4 boneIndex : BLENDINDICES;
	float4 blendWeight : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 uv : TEXCOORD0;
	float4 worldPos : TEXCOORD1;
};

struct VS_DEBUG_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 uv : TEXCOORD0;
	float4 worldPos : TEXCOORD1;
	float4 shadowPos : TEXCOORD2;
};

struct VS_SHADOW_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4	color		: SV_TARGET0;
	float4	normalMap	: SV_TARGET1;
	float4	normal		: SV_TARGET2;
	float4	tangent		: SV_TARGET3;
	float4	depthAndSpecular	: SV_TARGET4;
	int4	material	: SV_TARGET5;
};

Texture2D<float4> colorTexture : register(t10);
Texture2D<float4> normalTexture : register(t11);
Texture2D<float4> specularTexture : register(t12);
sampler Sampler : register(s0);

VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	float4 position = In.pos;
	position.w = 1.0f;
	Out.pos = mul(mvp, position);
	Out.worldPos = Out.pos;
	Out.pos = mul(viewProj, Out.pos);
	Out.normal = mul(mvp, In.normal);
	Out.normal = normalize(Out.normal);
	Out.tangent = mul(mvp, In.tangent);
	Out.tangent = normalize(Out.tangent);
	Out.binormal = cross(Out.normal, Out.tangent);
	Out.binormal = normalize(Out.binormal);
	Out.uv = In.uv;
	return Out;
}

VS_OUTPUT VSSkinMain(VS_SKIN_INPUT In)
{
	VS_OUTPUT Out;
	float4x4 pos = 0;
	float4 blendWeight;
	float weight = 0.0f;
	for (int i = 0;i < 3;i++)
	{
		pos += boneMatrix[In.boneIndex[i]] * In.blendWeight[i];
		weight += In.blendWeight[i];
	}
	pos += boneMatrix[In.boneIndex[3]] * (1.0f - weight);
	float4 position = In.pos;
	position.w = 1.0f;
	Out.pos = mul(pos, position);
	Out.worldPos = Out.pos;
	Out.pos = mul(viewProj, Out.pos);
	Out.normal = mul(pos, In.normal);
	Out.normal = normalize(Out.normal);
	Out.tangent = mul(pos, In.tangent);
	Out.tangent = normalize(Out.tangent);
	Out.binormal = cross(Out.normal, Out.tangent);
	Out.binormal = normalize(Out.binormal);
	Out.uv = In.uv;
	return Out;
}

PS_OUTPUT PSMain(VS_OUTPUT In)
{
	PS_OUTPUT Out;
	Out.color = float4(colorTexture.Sample(Sampler, In.uv).xyz, ambientLight.w);
	Out.normal = float4(In.normal, 1.0f);
	Out.tangent = float4(In.tangent, 1.0f);
	float3 normalColor = normalTexture.Sample(Sampler, In.uv);// * isNormalMap + float3(0.0f, 0.0f, 1.0f) * (1 - isNormalMap);
	Out.normalMap = float4(normalColor, 1.0f);
	float4 worldPos = mul(viewProj, In.worldPos);
	Out.depthAndSpecular.x = worldPos.z / worldPos.w;
	Out.depthAndSpecular.y = length(specularTexture.Sample(Sampler, In.uv).xyz);
	Out.depthAndSpecular.z = specularPower;
	Out.material.xyzw = 0;
	Out.material.x |= isShadowReceiver;
	Out.material.x |= isNormalMap;
	Out.material.x |= isDiffuse;
	Out.material.x |= isSpecularMap;
	return Out;
}

VS_SHADOW_OUTPUT VSShadowMain(VS_INPUT In)
{
	VS_SHADOW_OUTPUT Out;
	Out.pos = mul(mvp, In.pos);
	Out.pos = mul(lightViewProj, float4(Out.pos.xyz, 1.0f));
	Out.worldPos = Out.pos;
	return Out;
}

VS_SHADOW_OUTPUT VSShadowSkinMain(VS_SKIN_INPUT In)
{
	VS_SHADOW_OUTPUT Out;
	float4x4 pos = 0;
	float4 blendWeight;
	float weight = 0.0f;
	for (int i = 0;i < 3;i++)
	{
		pos += boneMatrix[In.boneIndex[i]] * In.blendWeight[i];
		weight += In.blendWeight[i];
	}
	pos += boneMatrix[In.boneIndex[3]] * (1.0f - weight);
	Out.pos = mul(pos, In.pos);
	Out.pos = mul(lightViewProj, float4(Out.pos.xyz, 1.0f));
	Out.worldPos = Out.pos;
	return Out;
}

float4 PSShadowMain(VS_SHADOW_OUTPUT In) : SV_TARGET0
{
	float4 Out;
	Out.xyz = In.worldPos.z / In.worldPos.w;
	Out.w = 1.0f;
	return Out;
}

