// ==================================================================================
// STRUCTURES AND VARIABLES
// ==================================================================================
texture SourceMap0;
sampler SourceSampler0
{
	Texture = SourceMap0;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};

#if defined(ENABLE_BLUR) || defined(ENABLE_BLOOM)
float4 ThresholdViewportInv;
#endif

#if defined(COLOR_CONTRAST)
float4 ColorContrast;
#endif

#if defined(FILTER_GRAYSCALE)
float4x4 Filter = {
	0.299f, 0.587f, 0.184f, 0.0f,
	0.299f, 0.587f, 0.184f, 0.0f,
	0.299f, 0.587f, 0.184f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f };
#elif defined(FILTER_SEPIA)
float4x4 Filter = {
	0.393f, 0.769f, 0.189f, 0.0f,
	0.349f, 0.686f, 0.168f, 0.0f,
	0.272f, 0.534f, 0.131f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f };
#elif defined(FILTER_NEGATIVE)
float4x4 Filter = {
	-1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f };
#elif defined(FILTER_CUSTOM)
float4x4 Filter;
#endif

struct VS_INPUT
{
	float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
	float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

// ==================================================================================
// VERTEX SHADER
// ==================================================================================
VS_OUTPUT Common_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = Input.Position;
	Output.TexCoord = Input.TexCoord;

	return Output;
}

// ==================================================================================
// PIXEL SHADER - EXTRACT
// ==================================================================================
float4 Extract_ps_main(PS_INPUT Input) : COLOR0
{
	float4 extract = float4(1.0f, 0.0f, 0.0f, 1.0f);

#if defined(ENABLE_BLOOM)
	extract = tex2D(SourceSampler0, Input.TexCoord);

	extract.rgb = saturate((extract.rgb - ThresholdViewportInv.x) / (1.0f - ThresholdViewportInv.x));
#endif

	return extract;
}

// ==================================================================================
// PIXEL SHADER - HORIZONTAL BLUR
// ==================================================================================
#if defined(ENABLE_BLUR) || defined(ENABLE_BLOOM)
texture SourceMap1;
sampler SourceSampler1
{
	Texture = SourceMap1;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};

static const float Filter[7] = { 0.3f, 0.1f, 0.05f, 0.1f, 0.05f, 0.1f, 0.3f };



float4 Horizontal_Blur_ps_main(PS_INPUT Input) : COLOR0
{
	float4 blur = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		blur += Filter[i] * tex2D(SourceSampler1, float2(Input.TexCoord.x + ((float)(i - 3) * ThresholdViewportInv.y), Input.TexCoord.y));
	}

	return blur;
}
#endif

// ==================================================================================
// PIXEL SHADER - VERTICAL BLUR
// ==================================================================================
#if defined(ENABLE_BLUR) || defined(ENABLE_BLOOM)
texture SourceMap2;
sampler SourceSampler2
{
	Texture = SourceMap2;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};

float4 Vertical_Blur_ps_main(PS_INPUT Input) : COLOR0
{
	float4 blur = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		blur += Filter[i] * tex2D(SourceSampler2, float2(Input.TexCoord.x, Input.TexCoord.y + ((float)(i - 3) * ThresholdViewportInv.z)));
	}

	return blur;
}
#endif

// ==================================================================================
// PIXEL SHADER - FINAL
// ==================================================================================
#if defined(ENABLE_BLOOM)
texture SourceMap3;
sampler SourceSampler3
{
	Texture = SourceMap3;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};
#endif

float4 Final_ps_main(PS_INPUT Input) : COLOR0
{
#if defined(ENABLE_BLUR) && !defined(ENABLE_BLOOM)
	float4 scene = tex2D(SourceSampler3, Input.TexCoord);
#else
	float4 scene = tex2D(SourceSampler0, Input.TexCoord);
#endif

#if defined(ENABLE_BLOOM)
	scene = saturate(scene + tex2D(SourceSampler3, Input.TexCoord));
#endif

#if defined(COLOR_CONTRAST)
	scene.rgb *= ColorContrast.rgb;

	float factor = (1.015625f * (ColorContrast.w + 1.0f)) / (1.0f * (1.015625f - ColorContrast.w));

	scene.r = saturate(factor * (scene.r - 0.5f) + 0.5f);
	scene.g = saturate(factor * (scene.g - 0.5f) + 0.5f);
	scene.b = saturate(factor * (scene.b - 0.5f) + 0.5f);
#endif

#if defined(FILTER_GRAYSCALE) || defined(FILTER_SEPIA) || defined(FILTER_NEGATIVE) || defined(FILTER_CUSTOM)
	scene = mul(Filter, scene);
#endif

	return scene;
}

// ==================================================================================
// RENDER SETTINGS
// ==================================================================================
technique CameraShader
{
	pass Extract
	{
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 Common_vs_main();
		PixelShader = compile ps_3_0 Extract_ps_main();
	}

	pass HorizontalBlur
	{
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 Common_vs_main();
		PixelShader = compile ps_3_0 Horizontal_Blur_ps_main();
	}

	pass VerticalBlur
	{
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 Common_vs_main();
		PixelShader = compile ps_3_0 Vertical_Blur_ps_main();
	}

	pass Final
	{
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 Common_vs_main();
		PixelShader = compile ps_3_0 Final_ps_main();
	}
}

