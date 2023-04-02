// Baeic Texture Shader
#type vertex

#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_WorldTransform;
			
out vec2 v_TexCoord;
void main(){
	v_TexCoord = a_TexCoord;

	gl_Position = u_ViewProjection * u_WorldTransform * vec4(a_Position, 1.0);
}
#type fragment

#version 330 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 color2;
uniform sampler2D u_Texture;			
uniform vec2 u_TexTiling;
uniform vec4 u_Color;

in vec2 v_TexCoord;
void main(){

	color = texture(u_Texture, v_TexCoord * u_TexTiling) * u_Color;
	color2 = vec4(1.0f,0.0f,0.0f,1.0f);
}