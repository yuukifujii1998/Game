
cbuffer cb : register(b0)
{
	float4x4 mvp;	//ワールドビュープロジェクション行列
	float alpha;
	float depthValue;
};

struct VS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);
sampler Sampler : register(s0);

VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = mul(mvp, In.pos);
	Out.pos.z = depthValue;
	Out.uv = In.uv;
	return Out;
}

float4 PSMain(VS_OUTPUT In) : SV_TARGET
{
	float4 color = colorTexture.Sample(Sampler, In.uv);
	clip(color.w - 0.1f);
	color.w = alpha;
	return color;
}
