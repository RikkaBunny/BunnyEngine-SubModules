// Baeic Texture Shader
#type vertex

#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in vec2 a_TexCoord0;


uniform mat4 u_ViewProjection;
uniform mat4 u_WorldTransform;
			
void main(){

	gl_Position = u_ViewProjection * u_WorldTransform * vec4(a_Position, 1.0);
}
#type fragment

#version 330 core
			
layout(location = 0) out int color;
//layout(location = 1) out int color2;

uniform int u_EntityID;

void main(){
//	float r = sin(u_EntityID*234);
//	float g = sin(u_EntityID*630);
//	float b = sin(u_EntityID*310);
//
//	color = vec4(r,g,b,1);
	color = u_EntityID;

}