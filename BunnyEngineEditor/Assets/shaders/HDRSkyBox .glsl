// Base Deferred Shader
#parameter
{
sampler2D u_AlbedoTexture = white;	

vec2 u_TexTiling=(1.0,1.0);
vec2 u_TexOffset=(0.0,0.0);

vec3 u_Emissive = (1,1,1);

}

#type vertex


#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in vec2 a_TexCoord0;


uniform mat4 u_ViewProjection;
uniform mat4 u_WorldTransform;
			
out vec3 v_TexCoords;


void main(){
	v_TexCoords = a_Position;

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


in vec3 v_TexCoords;


const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main(){

//	vec4 albedo = texture(u_AlbedoTexture, v_TexCoord * u_TexTiling);
//	vec3 envColor = textureLod(u_AlbedoTexture, (v_TexCoord+u_TexOffset) * u_TexTiling, 0.0).rgb;
	vec2 uv = SampleSphericalMap(normalize(v_TexCoords*vec3(u_TexTiling,1.0))); 
    vec3 color = texture(u_AlbedoTexture, uv).rgb;
//	envColor=pow(envColor,vec3(1.0/2.2));
	
	GBufferC = vec4(color*u_Emissive, 0);

	//GBufferD = v_WPos;
}