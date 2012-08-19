
float4x4 world_view_projection_matrix : VIEWPROJECTION;
float4x4 world_matrix : WORLD;

uniform extern texture diffuse_texture_0;
sampler DIFFUSE_TEXTURE_SAMPLER_0 = sampler_state { Texture = <diffuse_texture_0>; };

#if defined(TEXTURE_BLENDING_ENABLED)
uniform extern texture diffuse_texture_1;
uniform extern texture diffuse_texture_2;
uniform extern texture diffuse_texture_3;
sampler DIFFUSE_TEXTURE_SAMPLER_1 = sampler_state { Texture = <diffuse_texture_1>; };
sampler DIFFUSE_TEXTURE_SAMPLER_2 = sampler_state { Texture = <diffuse_texture_2>; };
sampler DIFFUSE_TEXTURE_SAMPLER_3 = sampler_state { Texture = <diffuse_texture_3>; };

#if defined(TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE)
uniform extern texture texture_blending_weights_texture;
sampler TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE_SAMPLER = sampler_state { Texture = <texture_blending_weights_texture>; };
#endif
#endif

#if defined(SKELETAL_ANIMATION_ENABLED)
#if !defined(MAX_JOINT_MATRICES)
#define MAX_JOINT_MATRICES 54
#endif

#if !defined(JOINTS_COUNT)
#define JOINTS_COUNT 8
#endif

float4x4 joint_matrix_array[MAX_JOINT_MATRICES] : WORLDMATRIXARRAY;

float3 ApplySkeletalAnimationToPosition(float3 p, const int joints[JOINTS_COUNT], const float weights[JOINTS_COUNT])
{
	float3 o = 0;

	for (int index = 0; index < JOINTS_COUNT; index++)
	{
		o += (float3) mul(float4(p, 1.0f), joint_matrix_array[joints[index]]) * weights[index];
	}

	return o;
}

float3 ApplySkeletalAnimationToVector(float3 v, const int joints[JOINTS_COUNT], const float weights[JOINTS_COUNT])
{
	float3 o = 0;

	for (int index = 0; index < JOINTS_COUNT; index++)
	{
		o += mul(v, (float3x3) joint_matrix_array[joints[index]]) * weights[index];
	}

	return o;
}
#endif

#if defined(LIGHTING_ENABLED)
#if !defined(MAX_POINT_LIGHTS)
#define MAX_POINT_LIGHTS 5
#endif

struct POINT_LIGHT
{
	float3 position;
	float3 color;
	float inv_attenuation;
};

uniform extern POINT_LIGHT point_lights[MAX_POINT_LIGHTS];

float3 GetPointLightIlluminationAtPosition(const int n, float3 position, float3 normal)
{
	float3 delta = point_lights[n].position - position;
	float power = saturate(point_lights[n].inv_attenuation * dot(delta, normal) / dot(delta, delta));
	return point_lights[n].color * power;
}

float3 GetPointLightsTotalIlluminationAtPosition(uniform const int count, float3 position, float3 normal)
{
	float3 color = GetPointLightIlluminationAtPosition(0, position, normal);

	for (int n = 1; n < count; n++)
	{
		color += GetPointLightIlluminationAtPosition(n, position, normal);
	}

	return color;
}
#endif

#if defined(NORMAL_MAP_ENABLED)
uniform extern texture  normal_map_texture;

sampler NORMAL_MAP_SAMPLER = sampler_state
{
	Texture = <normal_map_texture>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

float3 GetNormalMapNormal(const float2 texcoords, const float3 tangent, const float3 binormal, const float3 normal)
{
	float3 normal_tangent = normalize(2.0f * tex2D(NORMAL_MAP_SAMPLER, texcoords) - 1.0f);
    return mul(normal_tangent, float3x3(normalize(tangent), normalize(binormal), normalize(normal)));
}
#endif

struct VS_INPUT
{
	float3 position  : POSITION;

#if defined(VERTEX_TB_ENABLED)
    float3 tangent   : TANGENT0;
    float3 binormal  : BINORMAL0;
#endif

#if defined(VERTEX_N_ENABLED)
    float3 normal    : NORMAL0;
#endif

#if defined(VERTEX_JW_ENABLED)
    int4 joints1 : BLENDINDICES0;
    int4 joints2 : BLENDINDICES1;
    float4 weights1 : BLENDWEIGHT0;
	float4 weights2 : BLENDWEIGHT1;
#endif

    float2 texcoords : TEXCOORD0;

#if defined(TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX)
	float4 texture_blend_weights: TEXCOORD1;
#endif
};

struct VS_OUTPUT
{
	float4 position_h : POSITION;
	float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;

#if defined(VERTEX_TB_ENABLED)
	float3 tangent    : TEXCOORD2;
	float3 binormal   : TEXCOORD3;
#endif

#if defined(VERTEX_N_ENABLED)
	float3 normal     : TEXCOORD4;
#endif

#if defined(TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX)
	float4 texture_blend_weights: TEXCOORD5;
#endif
};


VS_OUTPUT VS(const VS_INPUT i)
{
    VS_OUTPUT o;

#if defined(VERTEX_JW_ENABLED)
	const int joints[8] = { i.joints1, i.joints2 };
	const float weights[8] = { i.weights1, i.weights2 };

	float3 position = ApplySkeletalAnimationToPosition(i.position, joints, weights);

    o.position_h = mul(float4(position, 1.0f), world_view_projection_matrix);
#else
    o.position_h = mul(float4(i.position, 1.0f), world_view_projection_matrix);
#endif

	o.position = mul(float4(i.position, 1.0f), world_matrix);

#if defined(VERTEX_JW_ENABLED) && defined(VERTEX_TB_ENABLED)
	float3 tangent = ApplySkeletalAnimationToVector(i.tangent, joints, weights);
	float3 binormal = ApplySkeletalAnimationToVector(i.binormal, joints, weights);

	o.tangent = normalize(mul(tangent, (float3x3) world_matrix));
	o.binormal = normalize(mul(binormal, (float3x3) world_matrix));
#endif

#if defined(VERTEX_JW_ENABLED) && defined(VERTEX_N_ENABLED)
	float3 normal = ApplySkeletalAnimationToVector(i.normal, joints, weights);
	o.normal = normalize(mul(normal, (float3x3) world_matrix));
#endif

#if !defined(VERTEX_JW_ENABLED) && defined(VERTEX_TB_ENABLED)
	o.tangent = normalize(mul(i.tangent, (float3x3) world_matrix));
	o.binormal = normalize(mul(i.binormal, (float3x3) world_matrix));
#endif

#if !defined(VERTEX_JW_ENABLED) && defined(VERTEX_N_ENABLED)
	o.normal = normalize(mul(i.normal, (float3x3) world_matrix));
#endif

    o.texcoords  = i.texcoords;

#if defined(TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX)
	o.texture_blend_weights = i.texture_blend_weights;
#endif

    return o;
}

float4 PS(const VS_OUTPUT i) : COLOR
{
#if defined(NORMAL_MAP_ENABLED)
	float3 normal = GetNormalMapNormal(i.texcoords, i.tangent, i.binormal, i.normal);
#else
#if defined(VERTEX_N_ENABLED)
	float3 normal = i.normal;
#endif
#endif

#if defined(LIGHTING_ENABLED)
	float4 light_color = float4(GetPointLightsTotalIlluminationAtPosition(MAX_POINT_LIGHTS, i.position, normal), 1.0f);
#endif

#if defined(TEXTURE_BLENDING_ENABLED)
#if defined(TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX)
	float4 blend_factor = i.texture_blend_weights;
#endif

#if defined(TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE)
	float4 blend_factor = tex2D(TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE_SAMPLER, i.texcoords);
#endif

	float4 color =
		tex2D(DIFFUSE_TEXTURE_SAMPLER_0, i.texcoords) * blend_factor.x +
		tex2D(DIFFUSE_TEXTURE_SAMPLER_1, i.texcoords) * blend_factor.y +
		tex2D(DIFFUSE_TEXTURE_SAMPLER_2, i.texcoords) * blend_factor.z +
		tex2D(DIFFUSE_TEXTURE_SAMPLER_3, i.texcoords) * blend_factor.w;
#else
	float4 color = tex2D(DIFFUSE_TEXTURE_SAMPLER_0, i.texcoords);
#endif

#if defined(LIGHTING_ENABLED)
	return light_color * color;
#else
	return color;
#endif
}

technique
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
    }
}

