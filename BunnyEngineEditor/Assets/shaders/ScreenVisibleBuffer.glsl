// Baeic Texture Shader
#type vertex

#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
			
out vec2 v_TexCoord;
void main(){
	v_TexCoord = a_TexCoord;

	gl_Position = vec4(a_Position.x*2.0f, a_Position.y*2.0f, 0.0, 1.0f); 
}
#type fragment

#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D u_GBufferA;
uniform sampler2D u_GBufferB;
uniform sampler2D u_GBufferC;
uniform sampler2D u_GBufferD;
uniform sampler2D u_DepthBuffer;

uniform int u_OutBufferType;

in vec2 v_TexCoord;

float near = 0.1; 
float far  = 1000.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main(){
	

	switch(u_OutBufferType){
	case 0:
		color = texture(u_GBufferA, v_TexCoord);
		color = vec4(color.x,color.y,color.z,1.0);
	break;
	case 1:
		color = texture(u_GBufferA, v_TexCoord);
		color = vec4(color.x,color.y,color.z,1.0);
	break;
	case 2:
		color = texture(u_DepthBuffer, v_TexCoord);
		float depth = LinearizeDepth(color.x) / far; 
		color = vec4(depth,depth,depth,1.0);
	break;
	case 3:
		color = texture(u_GBufferA, v_TexCoord);
		color = vec4(color.x,color.y,color.z,1.0);
	break;
	case 4:
		color = texture(u_GBufferB, v_TexCoord);
		color = vec4(color.x,color.y,color.z,1.0);
	break;
	case 5:
		color = texture(u_GBufferB, v_TexCoord);
		color = vec4(color.w,color.w,color.w,1.0);
	break;
	case 6:
		color = texture(u_GBufferA, v_TexCoord);
		color = vec4(color.w,color.w,color.w,1.0);
	break;
	case 7:
		color = texture(u_GBufferC, v_TexCoord);
		color = vec4(color.w,color.w,color.w,1.0);
	break;
	case 8:
		color = texture(u_GBufferC, v_TexCoord);
		color = vec4(color.x,color.y,color.z,1.0);
	break;
	}
}