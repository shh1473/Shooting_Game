// ==================================================================================
// STRUCTURES AND VARIABLES
// ==================================================================================
#if defined(DIFFUSE_MAP)
texture DiffuseMap;
sampler DiffuseSampler
{
	Texture = DiffuseMap;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};
#endif

#if defined(SPECULAR_MAP)
texture SpecularMap;
sampler SpecularSampler
{
	Texture = SpecularMap;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};
#endif

#if defined(NORMAL_MAP)
texture NormalMap;
sampler NormalSampler
{
	Texture = NormalMap;
	MINFILTER = NONE;
	MAGFILTER = NONE;
	MIPFILTER = NONE;
};
#endif

#if defined(AMBIENT_LIGHT)
struct AmbientLight
{
	float4 colorIntensity;
};
#endif

#if defined(DIRECTIONAL_LIGHT)
struct DirectionalLight
{
	float4 colorIntensity;
	float4 direction;
};
#endif

#if defined(POINT_LIGHT)
struct PointLight
{
	float4 colorIntensity;
	float4 translationRadius;
};
#endif

#if defined(SPOT_LIGHT)
struct SpotLight
{
	float4 colorIntensity;
	float4 direction;
	float2 coneAngle;
	float4 translationRadius;
};
#endif

float4x4 WVPMatrix;
float4 Color;

#if defined(TINT)
float4 Tint;
#endif

#if defined(AMBIENT_LIGHT) || defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
float4x4 WorldMatrix;
#endif

#if defined(AMBIENT_LIGHT)
AmbientLight AmbLight;
#endif

#if defined(DIRECTIONAL_LIGHT)
DirectionalLight DirLights[DIRECTIONAL_LIGHT_COUNT];
#endif

#if defined(POINT_LIGHT)
PointLight PntLights[POINT_LIGHT_COUNT];
#endif

#if defined(SPOT_LIGHT)
SpotLight SptLights[SPOT_LIGHT_COUNT];
#endif

#if defined(SPECULAR)
float4 CameraTranslation;
float SpecularIntensity;
float SpecularPower;
#endif

struct VS_INPUT
{
	float4 position : POSITION;
#if defined(DIFFUSE_MAP)
	float2 texCoord : TEXCOORD0;
#endif
#if defined(AMBIENT_LIGHT) || defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	float3 normal : NORMAL;
#endif
#if defined(NORMAL_MAP)
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
#endif
};

struct VS_OUTPUT
{
	float4 position : POSITION;
#if defined(DIFFUSE_MAP)
	float2 texCoord : TEXCOORD0;
#endif
#if defined(AMBIENT_LIGHT) || defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	float3 normal : TEXCOORD1;
#endif
#if defined(NORMAL_MAP)
	float3 tangent : TEXCOORD2;
	float3 binormal : TEXCOORD3;
#endif
#if defined(SPECULAR)
	float3 camDir : TEXCOORD4;
#endif
#if defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	float3 worldPos : TEXCOORD5;
#endif
};

struct PS_INPUT
{
#if defined(DIFFUSE_MAP)
	float2 texCoord : TEXCOORD0;
#endif
#if defined(AMBIENT_LIGHT) || defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	float3 normal : TEXCOORD1;
#endif
#if defined(NORMAL_MAP)
	float3 tangent : TEXCOORD2;
	float3 binormal : TEXCOORD3;
#endif
#if defined(SPECULAR)
	float3 camDir : TEXCOORD4;
#endif
#if defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	float3 worldPos : TEXCOORD5;
#endif
};

// ==================================================================================
// VERTEX SHADER
// ==================================================================================
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.position = mul(Input.position, WVPMatrix);

#if defined(DIFFUSE_MAP)
	Output.texCoord = Input.texCoord;
#endif

#if defined(AMBIENT_LIGHT) || defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	Output.normal = mul(Input.normal, (float3x3)WorldMatrix);
	Output.normal = normalize(Output.normal);
#endif

#if defined(NORMAL_MAP)
	Output.tangent = mul(Input.tangent, (float3x3)WorldMatrix);
	Output.tangent = normalize(Output.tangent);

	Output.binormal = mul(Input.binormal, (float3x3)WorldMatrix);
	Output.binormal = normalize(Output.binormal);
#endif

#if defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	Output.worldPos = mul(Input.position, WorldMatrix).xyz;
#endif

#if defined(SPECULAR)
#if defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	Output.camDir = Output.worldPos - CameraTranslation.xyz;
#else
	Output.camDir = mul(Input.position, WorldMatrix).xyz - CameraTranslation.xyz;
#endif
	Output.camDir = normalize(Output.camDir);
#endif

	return Output;
}

// ==================================================================================
// PIXEL SHADER
// ==================================================================================
float4 ps_main(PS_INPUT Input) : COLOR0
{
#if defined(AMBIENT_LIGHT) || defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	float3 finalColor = 0.0f;
#else
	float3 finalColor = 1.0f;
#endif

	float finalAlpha = 1.0f;

#if defined(DIFFUSE_MAP)
	float4 diffuseMap = tex2D(DiffuseSampler, Input.texCoord);

	finalAlpha *= diffuseMap.a;
	finalAlpha = saturate(finalAlpha);
#endif

#if defined(SPECULAR_MAP)
	float4 specularMap = tex2D(SpecularSampler, Input.texCoord);
#endif

#if defined(AMBIENT_LIGHT) || defined(DIRECTIONAL_LIGHT) || defined(POINT_LIGHT) || defined(SPOT_LIGHT)
	float3 normal = 0.0f;

#if defined(NORMAL_MAP)
	normal = tex2D(NormalSampler, Input.texCoord).xyz;
	normal = (normal * 2.0f) - 1.0f;
	normal = Input.normal + (Input.tangent * normal.x) + (Input.binormal * normal.y);
	normal = normalize(normal);
#else
	normal = Input.normal;
#endif
#endif

#if defined(AMBIENT_LIGHT)
	float3 ambient = AmbLight.colorIntensity.rgb;
	ambient *= AmbLight.colorIntensity.w;

	finalColor += diffuseMap.rgb * ambient;
	finalColor = saturate(finalColor);
#endif

#if defined(DIRECTIONAL_LIGHT)
	float3 dirTotal = 0.0f;
	float3 tmpDirDiffuse = 0.0f;
	float3 tmpDirSpecular = 0.0f;

#if (DIRECTIONAL_LIGHT_COUNT > 1)
	for (int i = 0; i < DIRECTIONAL_LIGHT_COUNT; ++i)
#else
	int i = 0;
#endif
	{
		tmpDirDiffuse = dot(-DirLights[i].direction, normal);
		tmpDirDiffuse = saturate(tmpDirDiffuse);

#if defined(SPECULAR)
		if (tmpDirDiffuse.x > 0.0f)
		{
			float3 refDir = reflect(DirLights[i].direction, normal);
			refDir = normalize(refDir);

			tmpDirSpecular = dot(refDir, -Input.camDir);
			tmpDirSpecular = saturate(tmpDirSpecular);

			tmpDirSpecular = pow(tmpDirSpecular, SpecularPower);
#if defined(SPECULAR_MAP)
			tmpDirSpecular *= specularMap.rgb;
#endif
			tmpDirSpecular *= SpecularIntensity;
			tmpDirSpecular = saturate(tmpDirSpecular);

			tmpDirDiffuse += tmpDirSpecular;
			tmpDirDiffuse = saturate(tmpDirDiffuse);
		}
#endif

		tmpDirDiffuse *= DirLights[i].colorIntensity.w;
		tmpDirDiffuse *= DirLights[i].colorIntensity.rgb;

		dirTotal += tmpDirDiffuse;
	}

	finalColor += diffuseMap.rgb * dirTotal;
	finalColor = saturate(finalColor);
#endif

#if defined(POINT_LIGHT)
	float3 pntTotal = 0.0f;
	float3 tmpPntDiffuse = 0.0f;
	float3 tmpPntSpecular = 0.0f;

#if (POINT_LIGHT_COUNT > 1)
	for (int i = 0; i < POINT_LIGHT_COUNT; ++i)
#else
	int i = 0;
#endif
	{
		float3 lightDir = Input.worldPos - PntLights[i].translationRadius.xyz;
		float lightDist = length(lightDir);
		lightDir /= lightDist;

		float attenuation = 1.0f - saturate(lightDist / PntLights[i].translationRadius.w);

		if (attenuation > 0.0f)
		{
			tmpPntDiffuse = dot(-lightDir, normal);
			tmpPntDiffuse = saturate(tmpPntDiffuse);

#if defined(SPECULAR)
			if (tmpPntDiffuse.x > 0.0f)
			{
				float3 refDir = reflect(lightDir, normal);
				refDir = normalize(refDir);

				tmpPntSpecular = dot(refDir, -Input.camDir);
				tmpPntSpecular = saturate(tmpPntSpecular);

				tmpPntSpecular = pow(tmpPntSpecular, SpecularPower);
#if defined(SPECULAR_MAP)
				tmpPntSpecular *= specularMap.rgb;
#endif
				tmpPntSpecular *= SpecularIntensity;
				tmpPntSpecular = saturate(tmpPntSpecular);

				tmpPntDiffuse += tmpPntSpecular;
				tmpPntDiffuse = saturate(tmpPntDiffuse);
			}
#endif

			tmpPntDiffuse *= PntLights[i].colorIntensity.w;
			tmpPntDiffuse *= PntLights[i].colorIntensity.rgb;

			pntTotal += tmpPntDiffuse * attenuation;
		}
	}

	finalColor += diffuseMap.rgb * pntTotal;
	finalColor = saturate(finalColor);
#endif

#if defined(SPOT_LIGHT)
	float3 sptTotal = 0.0f;
	float3 tmpSptDiffuse = 0.0f;
	float3 tmpSptSpecular = 0.0f;

#if (SPOT_LIGHT_COUNT > 1)
	for (int i = 0; i < SPOT_LIGHT_COUNT; ++i)
#else
	int i = 0;
#endif
	{
		float3 lightDir = Input.worldPos - SptLights[i].translationRadius.xyz;
		float lightDist = length(lightDir);
		lightDir /= lightDist;

		float attenuation = 1.0f - saturate(lightDist / SptLights[i].translationRadius.w);

		if (attenuation > 0.0f)
		{
			float coneAttenuation = saturate((dot(SptLights[i].direction, lightDir) - SptLights[i].coneAngle.y) / SptLights[i].coneAngle.x);

			if (coneAttenuation > 0.0f)
			{
				tmpSptDiffuse = dot(-lightDir, normal);
				tmpSptDiffuse = saturate(tmpSptDiffuse);

#if defined(SPECULAR)
				if (tmpSptDiffuse.x > 0.0f)
				{
					float3 refDir = reflect(lightDir, normal);
					refDir = normalize(refDir);

					tmpSptSpecular = dot(refDir, -Input.camDir);
					tmpSptSpecular = saturate(tmpSptSpecular);

					tmpSptSpecular = pow(tmpSptSpecular, SpecularPower);
#if defined(SPECULAR_MAP)
					tmpSptSpecular *= specularMap.rgb;
#endif
					tmpSptSpecular *= SpecularIntensity;
					tmpSptSpecular = saturate(tmpSptSpecular);

					tmpSptDiffuse += tmpSptSpecular;
					tmpSptDiffuse = saturate(tmpSptDiffuse);
				}
#endif

				tmpSptDiffuse *= SptLights[i].colorIntensity.w;
				tmpSptDiffuse *= SptLights[i].colorIntensity.rgb;

				sptTotal += tmpSptDiffuse * attenuation * coneAttenuation;
			}
		}
	}

	finalColor += diffuseMap.rgb * sptTotal;
	finalColor = saturate(finalColor);
#endif

	finalColor *= Color.rgb;
	finalColor = saturate(finalColor);

	finalAlpha *= Color.a;
	finalAlpha = saturate(finalAlpha);

#if defined(TINT)
	finalColor *= Tint.rgb;
	finalColor = saturate(finalColor);

	finalAlpha *= Tint.a;
	finalAlpha = saturate(finalAlpha);
#endif

	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	return float4(pow(finalColor, 2.2f), finalAlpha);
}

// ==================================================================================
// RENDER SETTINGS
// ==================================================================================
technique SpriteShader
{
	pass Sprite
	{
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		ALPHABLENDENABLE = TRUE;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}