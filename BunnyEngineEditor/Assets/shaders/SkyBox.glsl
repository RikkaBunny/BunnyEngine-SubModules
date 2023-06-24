// Base Deferred Shader
#parameter
{
sampler2D u_AlbedoTexture = white;	

vec2 u_TexTiling=(1.0,1.0);
vec2 u_TexOffset=(0.0,0.0);

vec3 u_Emissive = (1,1,1);

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
out vec4 v_WPos;

void main(){
	v_TexCoord = a_TexCoord0;
	vec4 WorldNormal = normalize(u_WorldTransform * vec4(a_Normal, 1.0f));
	vec3 B = normalize(cross(a_Tangent, WorldNormal.xyz));

	v_TBN = mat3(a_Tangent, B, vec3(WorldNormal.xyz));

	v_WPos =  u_WorldTransform * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * u_WorldTransform * vec4(a_Position, 1.0);
}
#type fragment

#version 330 core
			
//layout(location = 0) out vec4 GBufferA;
//layout(location = 1) out vec4 GBufferB;
layout(location = 2) out vec4 GBufferC;
//layout(location = 3) out vec4 GBufferD;

uniform sampler2D u_AlbedoTexture;	

uniform vec2 u_TexTiling;
uniform vec2 u_TexOffset;

uniform vec3 u_Emissive;


in vec2 v_TexCoord;
in mat3 v_TBN;
in vec4 v_WPos;

void main(){

//	vec4 albedo = texture(u_AlbedoTexture, v_TexCoord * u_TexTiling);
	vec3 envColor = textureLod(u_AlbedoTexture, (v_TexCoord+u_TexOffset) * u_TexTiling, 0.0).rgb;
//	envColor=envColor/(envColor+vec3(1.0));
	envColor=pow(envColor,vec3(1.0/2.2));
	
	GBufferC = vec4(envColor*u_Emissive, 0);

	//GBufferD = v_WPos;
}