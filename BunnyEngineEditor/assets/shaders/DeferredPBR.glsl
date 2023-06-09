// Base Deferred Shader
#parameter
{
sampler2D u_AlbedoTexture = white;	
sampler2D u_NormalTexture = blue;	
sampler2D u_MetallicTexture = black;	
sampler2D u_RoughnessTexture = gray;	
sampler2D u_AoTexture = white;	

uniform vec2 u_TexTiling=(1.0,1.0);
uniform vec4 u_Color = (1,0,0,1);
uniform float u_Metallic = 0;
uniform float u_Roughness = 0.5;
uniform float u_Emissive = 1;

uniform int u_EntityID = 11;
}

#type vertex

//layout(location = 0) in vec3 a_Position;
//layout(location = 1) in vec3 a_Normal;
//layout(location = 2) in vec3 a_Tangent;
//layout(location = 3) in vec3 a_Color;
//layout(location = 4) in vec2 a_TexCoord0;
//layout(location = 5) in vec2 a_TexCoord1;
//layout(location = 6) in vec2 a_TexCoord2;
//layout(location = 7) in vec2 a_TexCoord3;
//layout(location = 8) in vec2 a_TexCoord4;
//layout(location = 9) in vec2 a_TexCoord5;
//layout(location = 10) in vec2 a_TexCoord6;
//layout(location = 11) in vec2 a_TexCoord7;
//layout(location = 12) in vec2 a_TexCoord8;

#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in vec2 a_TexCoord0;


uniform mat4 u_ViewProjection;
uniform mat4 u_WorldTransform;
			
out vec2 v_TexCoord;
out mat3 v_TBN;

void main(){
	v_TexCoord = a_TexCoord0;
	vec4 WorldNormal = normalize(u_WorldTransform * vec4(a_Normal, 1.0f));
	vec3 B = normalize(cross(a_Tangent, WorldNormal.xyz));

	v_TBN = mat3(a_Tangent, B, vec3(WorldNormal.xyz));

	gl_Position = u_ViewProjection * u_WorldTransform * vec4(a_Position, 1.0);
}
#type fragment

#version 330 core
			
layout(location = 0) out vec4 GBufferA;
layout(location = 1) out vec4 GBufferB;
layout(location = 2) out vec4 GBufferC;
layout(location = 3) out int EntityID;

uniform sampler2D u_AlbedoTexture;	
uniform sampler2D u_NormalTexture;
uniform sampler2D u_MetallicTexture;
uniform sampler2D u_RoughnessTexture;
uniform sampler2D u_AoTexture;

uniform vec2 u_TexTiling;
uniform vec4 u_Color;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_Emissive;

uniform int u_EntityID;

in vec2 v_TexCoord;
in mat3 v_TBN;
void main(){

	vec4 albedo = texture(u_AlbedoTexture, v_TexCoord * u_TexTiling) * u_Color;
	vec3 normal = texture(u_NormalTexture, v_TexCoord * u_TexTiling).xyz;
	normal=normalize(normal*2.0-1.0);
	normal=normalize(v_TBN*normal);
	float metallic = texture(u_MetallicTexture, v_TexCoord * u_TexTiling).g * u_Metallic;
	float roughness = texture(u_RoughnessTexture, v_TexCoord * u_TexTiling).g * u_Roughness;
	float ao = texture(u_AoTexture, v_TexCoord * u_TexTiling).g;

	//BufferA16(8)f  BaseRGB + Emissive  
	GBufferA = vec4(albedo.rgb, u_Emissive);
	//BufferB16f WNormalRGB roughness  A 
	GBufferB = vec4(normal.rgb, roughness);
	//BufferC8f R metallic G specular B ao A customdata
	GBufferC = vec4(metallic, 0.5f, ao, 1.0f);

	EntityID = u_EntityID;
}