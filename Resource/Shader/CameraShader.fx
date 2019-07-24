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

#if defined(BLUR) || defined(BLOOM)
texture SourceMap1;
sampler SourceSampler1
{
	Texture = SourceMap1;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};
#endif

#if defined(BLUR) || defined(BLOOM)
texture SourceMap2;
sampler SourceSampler2
{
	Texture = SourceMap2;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};
#endif

#if defined(BLUR) || defined(BLOOM)
texture SourceMap3;
sampler SourceSampler3
{
	Texture = SourceMap3;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};
#endif

float4 ThresholdIntensityViewportInv;

#if defined(BLUR) || defined(BLOOM)
static const float BlurKernel[7] = { 0.05f, 0.1f, 0.2f, 0.3f, 0.2f, 0.1f, 0.05f };
#endif

#if defined(COLOR_CONTRAST)
float4 ColorContrast;
#endif

#if defined(FILTER)
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
	Output.TexCoord = Input.TexCoord + ThresholdIntensityViewportInv.zw;

	return Output;
}

// ==================================================================================
// PIXEL SHADER - EXTRACT
// ==================================================================================
#if defined(BLOOM)
float4 Extract_ps_main(PS_INPUT Input) : COLOR0
{
	float4 extract = tex2D(SourceSampler0, Input.TexCoord);

	extract.rgb = saturate((extract.rgb - ThresholdIntensityViewportInv.x) / (1.0f - ThresholdIntensityViewportInv.x));

	return extract;
}
#endif

// ==================================================================================
// PIXEL SHADER - HORIZONTAL BLUR
// ==================================================================================
#if defined(BLUR) || defined(BLOOM)
float4 Horizontal_Blur_ps_main(PS_INPUT Input) : COLOR0
{
	float4 blur = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		float coordX = Input.TexCoord.x + ((float)(i - 3) * ThresholdIntensityViewportInv.z);

		coordX = clamp(coordX, 0.0f, 1.0f);

		blur += BlurKernel[i] * tex2D(SourceSampler1, float2(coordX, Input.TexCoord.y));
	}

	return blur;
}
#endif

// ==================================================================================
// PIXEL SHADER - VERTICAL BLUR
// ==================================================================================
#if defined(BLUR) || defined(BLOOM)
float4 Vertical_Blur_ps_main(PS_INPUT Input) : COLOR0
{
	float4 blur = 0.0f;

	for (int i = 0; i < 7; ++i)
	{
		float coordY = Input.TexCoord.y + ((float)(i - 3) * ThresholdIntensityViewportInv.w);

		coordY = clamp(coordY, 0.0f, 1.0f);

		blur += BlurKernel[i] * tex2D(SourceSampler2, float2(Input.TexCoord.x, coordY));
	}

	return blur;
}
#endif

// ==================================================================================
// PIXEL SHADER - FINAL
// ==================================================================================
float4 Final_ps_main(PS_INPUT Input) : COLOR0
{
#if defined(BLUR) && !defined(BLOOM)
	float4 scene = tex2D(SourceSampler3, Input.TexCoord);
#else
	float4 scene = tex2D(SourceSampler0, Input.TexCoord);
#endif

#if defined(BLOOM)
	float4 bloom = tex2D(SourceSampler3, Input.TexCoord);

	bloom.rgb *= ThresholdIntensityViewportInv.y;

	scene = saturate(scene + bloom);
#endif

#if defined(COLOR_CONTRAST)
	scene.rgb *= ColorContrast.rgb;

	float factor = (1.015625f * (ColorContrast.w + 1.0f)) / (1.0f * (1.015625f - ColorContrast.w));

	scene.r = saturate(factor * (scene.r - 0.5f) + 0.5f);
	scene.g = saturate(factor * (scene.g - 0.5f) + 0.5f);
	scene.b = saturate(factor * (scene.b - 0.5f) + 0.5f);
#endif

#if defined(FILTER)
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
#if defined(BLOOM)
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 Common_vs_main();
		PixelShader = compile ps_3_0 Extract_ps_main();
#endif
	}

	pass HorizontalBlur
	{
#if defined(BLUR) || defined(BLOOM)
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 Common_vs_main();
		PixelShader = compile ps_3_0 Horizontal_Blur_ps_main();
#endif
	}

	pass VerticalBlur
	{
#if defined(BLUR) || defined(BLOOM)
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 Common_vs_main();
		PixelShader = compile ps_3_0 Vertical_Blur_ps_main();
#endif
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

