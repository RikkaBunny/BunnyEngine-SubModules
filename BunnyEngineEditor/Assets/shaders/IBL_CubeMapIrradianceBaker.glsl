
#type vertex


#version 330 core
			
layout(location = 0) in vec3 a_Position;
			
out vec3 v_TexCoords;

uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;

void main()
{
    v_TexCoords = a_Position;
	vec4 position = u_ProjMat * (u_ViewMat ) * vec4(a_Position, 1.0);
    gl_Position = position.xyww;
}
#type fragment

#version 330 core
			
layout(location = 0) out vec4 FragColor;

in vec3 v_TexCoords;

//uniform samplerCube u_Skybox;
uniform sampler2D u_Skybox;
const float PI=3.14;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{       
    vec3 normal=normalize(v_TexCoords);
	vec3 irradiance=vec3(0.0);

	vec3 up=vec3(0,1.0,0);
	vec3 right=cross(normal,up);
	up=cross(right,normal);

	float sampleDelta=0.025;
	float nrSamples=0.0;
	for(float phi=0.0;phi < 2.0*PI;phi+=sampleDelta){
		for(float theta=0.0;theta < 0.5*PI;theta+=sampleDelta){
			//球面上根据角度采样点的坐标 
			vec3 tangentSample=vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			//转到世界空间
			vec3 sampleVec=tangentSample.x*right+tangentSample.y*up+tangentSample.z*normal;
			//根据向量采样hdrSkyBox  *cos是入射光强度衰减  *sin是球顶区域太密集减小顶部贡献度，增加水平贡献
			vec2 uv = SampleSphericalMap(normalize(sampleVec)); 
			irradiance+=texture(u_Skybox,uv).rgb*cos(theta)*sin(theta);
			nrSamples++;
		}
	}
	//积分漫反射部分
	irradiance=PI*irradiance*(1.0/float(nrSamples));
	FragColor=vec4(irradiance , 1.0);
//	FragColor=vec4(1.0,0,0 , 1.0);
}