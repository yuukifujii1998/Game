
cbuffer lightCB : register(b0)
{
	float4 ambientLight;
	float4 diffuseLight[4];
	float4 diffuseLightDir[4];
};

cbuffer shadowConB : register(b1)
{
	float4x4 gameView;
	float4x4 gameProj;
};

cbuffer shadowCB : register(b2)
{
	float4x4 lightViewProj1;
	float4x4 lightViewProj2;
	float4x4 lightViewProj3;
};

cbuffer materialCB : register(b3)
{
	int isShadowReceiver;
	int isNormalMapFlg;
	int isDiffuseFlg;
	int isSpecularMap;
};


cbuffer framesizeCB : register(b4)
{
	float4 gameCameraPos;
	int frameBufferWidth;
	int frameBufferHeight;
};
struct SPointLight
{
	float4 pos;
	float4 color;
};

StructuredBuffer<SPointLight> pointLightList : register(t10);


struct VS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 screenPos : TEXCOORD1;
	float2 uv : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);
Texture2D<float4> normalMapTexture : register(t1);
Texture2D<float4> normalTexture : register(t2);
Texture2D<float4> tangentTexture : register(t3);
Texture2D<float4> depthTexture : register(t4);
Texture2D<int4> materialTexture : register(t5);
Texture2D<float4> velocityTexture : register(t6);
Texture2D<float4> shadowTexture1 : register(t7);
Texture2D<float4> shadowTexture2 : register(t8);
Texture2D<float4> shadowTexture3 : register(t9);
sampler Sampler : register(s0);
sampler shadowSampler : register(s1);

VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = In.pos;
	Out.uv = In.uv;
	Out.screenPos = In.pos;
	return Out;
}


int ShadowMapCalc(VS_OUTPUT In, Texture2D<float4> shadowTexture, float4x4 lightViewProj,  float maindepth)
{
	float4 shadowMapPos;
	shadowMapPos.z = maindepth;
	shadowMapPos.xy = In.uv;
	shadowMapPos.y = 1.0f - shadowMapPos.y;
	shadowMapPos.xy -= 0.5f;
	shadowMapPos.xy *= 2.0f;
	shadowMapPos.w = 1.0f;
	shadowMapPos = mul(gameProj, shadowMapPos);
	shadowMapPos = mul(gameView, shadowMapPos);
	shadowMapPos /= shadowMapPos.w;
	float3 worldPos = shadowMapPos.xyz;
	shadowMapPos = mul(lightViewProj, shadowMapPos);
	shadowMapPos /= shadowMapPos.w;
	shadowMapPos.xy += 1.0f;
	shadowMapPos.xy /= 2.0f;
	shadowMapPos.y = 1.0f - shadowMapPos.y;
	float depth = shadowMapPos.z;
	float shadowDepth;
	shadowDepth = shadowTexture.Sample(shadowSampler, shadowMapPos.xy).x;
	int shadowValue = 1;
	if (shadowMapPos.x <= 1.0f && 0.0f <= shadowMapPos.x
		&&	shadowMapPos.y <= 1.0f && 0.0f <= shadowMapPos.y)
	{
		float d = depth - 0.01 - shadowDepth;
		shadowValue *= step(depth, shadowDepth + 0.00001f);
	}
	return shadowValue;
}

float4 PSMain(VS_OUTPUT In) : SV_TARGET0
{
	float4 color = colorTexture.Sample(Sampler, In.uv);
	float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
	normal = normalize(normal);
	float3 tangent = tangentTexture.Sample(Sampler, In.uv).xyz;
	float3 binormal = cross(normal, tangent).xyz;
	normal = normalize(normal);
	tangent = normalize(tangent);
	binormal = normalize(binormal);
	float3 normalColor = normalMapTexture.Sample(Sampler, In.uv).xyz;
	int3 materialUV;
	materialUV.z = 0;
	materialUV.x = (int)(In.uv.x * 1280.0f);
	materialUV.y = (int)(In.uv.y * 720.0f);
	int4 materialFlg = materialTexture.Load(materialUV, int2(0, 0));
	
	float4x4 mat = {
		float4(tangent, 0.0f),
		float4(binormal, 0.0f),
		float4(normal, 0.0f),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	normalColor = mul(normalColor, mat);
	float3 normalLight[4] =
	{
		normalize(diffuseLightDir[0].xyz),
		normalize(diffuseLightDir[1].xyz),
		normalize(diffuseLightDir[2].xyz),
		normalize(diffuseLightDir[3].xyz)
	};
	
	float4 depthAndSpecular = depthTexture.Sample(Sampler, In.uv);
	float4 lig = float4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		lig.xyz += diffuseLight[i].xyz * max(-dot(normal, normalLight[i]), 0.0f) * step(1, !(materialFlg.x & isNormalMapFlg)) * step(1, (materialFlg.x & isDiffuseFlg)) * depthAndSpecular.w;
		lig.xyz	+= diffuseLight[i].xyz * max(-dot(normalColor, normalLight[i]), 0.0f) * step(1, (materialFlg.x & isNormalMapFlg)) * step(1, (materialFlg.x & isDiffuseFlg)) * depthAndSpecular.w;
	}
	lig.xyz += color.w;
	color.xyz *= lig;
	float4 shadowMapPos;
	shadowMapPos.z = depthAndSpecular.x;
	shadowMapPos.xy = In.uv;
	shadowMapPos.y = 1.0f - shadowMapPos.y;
	shadowMapPos.xy -= 0.5f;
	shadowMapPos.xy *= 2.0f;
	shadowMapPos.w = 1.0f;
	shadowMapPos = mul(gameProj, shadowMapPos);
	shadowMapPos = mul(gameView, shadowMapPos);
	shadowMapPos /= shadowMapPos.w;
	float3 worldPos = shadowMapPos.xyz;
	for(int i = 0;i < 4;i++)
	{
		float3 lineSight = worldPos - gameCameraPos.xyz;
		lineSight = normalize(lineSight);
		lineSight = reflect(lineSight, normal);
		lineSight = normalize(lineSight);
		float speculaLight = dot(lineSight, -normalLight[i]);
		speculaLight = max(speculaLight, 0.0f);
		speculaLight = pow(speculaLight, 10.0f);
		color.xyz += speculaLight * (depthAndSpecular.y * step(1, (materialFlg.x & isSpecularMap)) + step(1, !(materialFlg.x & isSpecularMap))) * depthAndSpecular.z;
	}
	float shadowValue = ShadowMapCalc(In, shadowTexture1, lightViewProj1, depthAndSpecular.x);
	shadowValue *= ShadowMapCalc(In, shadowTexture2, lightViewProj2, depthAndSpecular.x);
	shadowValue *= ShadowMapCalc(In, shadowTexture3, lightViewProj3, depthAndSpecular.x);
	shadowValue = min(1, step(1, !(materialFlg.x & isShadowReceiver)) + shadowValue);
	color.xyz = lerp(color.xyz * 0.5f, color.xyz, shadowValue);
	//color.xyz  = ShadowMapCalcTest(In, shadowTexture1, lightViewProj1, depthAndSpecular.x).xyz;
	

	return color;
}

