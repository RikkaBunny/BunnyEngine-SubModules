
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

uniform sampler2D u_EquirectangularMap;

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
    vec2 uv = SampleSphericalMap(normalize(v_TexCoords)); 
    vec3 color = texture(u_EquirectangularMap, uv).rgb;

    FragColor = vec4(color, 1.0);
//    FragColor = vec4(1,0,1, 1.0);
}