// Baeic Texture Shader

#parameter
{
float PI=3.14159265359;
vec2 u_TesTile = (0.5);
bool u_Bool = false;
}

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
			
layout(location = 0) out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D u_GBufferA;
uniform sampler2D u_GBufferB;
uniform sampler2D u_GBufferC;
uniform sampler2D u_GBufferD;
uniform sampler2D u_DepthBuffer;

uniform samplerCube u_irradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BRDFLUT;

layout(std140) uniform LightDirection{
	vec3 lightColor;
	vec3 lightDirector;
	vec3 lightPos;
};

uniform vec3 cameraPos;

const float PI=3.14159265359;

//计算（高光）反射的百分比
vec3 fresnelSchlick(float cosTheta,vec3 F0 ,float roughness){
	return F0+(max(vec3(1.0-roughness),F0)-F0)*pow(1.0-cosTheta,5.0);
}
//正态分布函数 估算微平面的总体取向度
float DistributionGGX(vec3 N,vec3 H,float roughness){
	//使用粗糙度平方会更好
	float a=roughness*roughness;
	float a2=a*a;
	float NdotH=max(dot(N,H),0.0);
	float NdotH2=NdotH*NdotH;
	float nom=a2;
	float denom=NdotH2*(a2-1.0)+1.0;
	denom=PI*(denom*denom);

	return nom/denom;
}
//计算遮蔽值
float GeometrySchlickGGX(float NdotV,float roughness){
	float r=roughness+1.0;
	float k=r*r/8;

	float nom=NdotV;
	float denom=NdotV*(1.0-k)+k;
	
	return nom/denom;
}
//计算观察方向（几何遮蔽值）和光线方向（几何阴影值）
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness){
	float NdotV=max(dot(N,V),0.0);
	float NdotL=max(dot(N,L),0.0);
	float ggx2=GeometrySchlickGGX(NdotV,roughness);
	float ggx1=GeometrySchlickGGX(NdotL,roughness);

	return ggx2*ggx1;
}


void main(){
    vec4 GBufferA = texture(u_GBufferA,v_TexCoord);
    vec4 GBufferB = texture(u_GBufferB,v_TexCoord);
    vec4 GBufferC = texture(u_GBufferC,v_TexCoord);
    vec4 GBufferD = texture(u_GBufferD,v_TexCoord);
	vec3 albedo=pow(GBufferA.rgb, vec3(2.2));
//	vec3 albedo=vec3(1.0);
	vec3 normal=GBufferB.rgb;
	vec3 wPos=GBufferD.rgb;
    vec3 emissive=GBufferC.rgb;
    
//	vec3 normal=fs_in.wNormal;
	float metallic=GBufferA.a;
	float roughness=GBufferB.a;
	float ao=GBufferC.a;
	
//	float ao=1;


	vec3 N=normalize(normal);
	vec3 V=normalize(cameraPos-wPos);

	vec3 Lo=vec3(0.0);


	vec3 L=normalize(lightDirector);
	vec3 H=normalize(V+L);

//	float distance=length(lightPointPos-wPos);
//	float attenuation=1.0 / (lightPointconstant + lightPointlinear * distance + lightPointquadratic * (distance * distance)); 
//	float attenuation=1.0 / distance*distance;
//	float attenuation=texture(u_GBufferA,v_TexCoord).a;
	vec3 radiance=lightColor;

	vec3 F0=vec3(0.04);
	F0=mix(F0,albedo,metallic);
	vec3 F=fresnelSchlick(max(dot(V,H),0.0),F0,roughness);

	float NDF=DistributionGGX(N,H,roughness);
	float G=GeometrySmith(N,V,L,roughness);

	vec3 nominator=NDF*G*F;
	//加0.001防止除以0 出错
	float denominator = 4.0 * (max(dot(N, V), 0.0)) * (max(dot(N, L), 0.0)) + 0.001; 
	vec3 specular=nominator/denominator;

	vec3 Ks=F;
	vec3 Kd=vec3(1.0)-Ks;
	//金属度越高越没有折射 金属没有折射
	Kd*=1.0-metallic;
	//环境光给予的漫反射
	vec3 irradiance = texture(u_irradianceMap, N).rgb;
	vec3 diffuse    = irradiance * albedo;
	
	
	//预计算的环境高光的DNF正太分布值
	vec3 R=reflect(-V,N);
	const float MAX_REFLECTION_LOD=4.0;
	vec3 prefilteredColor=textureLod(u_PrefilterMap,R,roughness*MAX_REFLECTION_LOD).rgb;

	vec2 envBRDF=texture(u_BRDFLUT,vec2(max(dot(N,V),0.0),roughness)).rg;
	vec3 envSpecular=prefilteredColor*(F*envBRDF.x+envBRDF.y);

	float NdotL=max(dot(N,L),0.0);
	Lo+=(Kd * albedo / PI + specular) * radiance *NdotL;
	vec3 ambient    = (Kd * diffuse+envSpecular) * ao; 
	vec3 color   = ambient + Lo;  
	//HDR gamma矫正
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 

	FragColor = vec4(color+emissive, 1.0);
//	vec3 envColor = textureLod(u_PrefilterMap, wPos, 1.2).rgb;
//	prefilteredColor=textureLod(u_PrefilterMap,R,roughness*MAX_REFLECTION_LOD).rgb;
//	FragColor = vec4(prefilteredColor, 1.0);
}