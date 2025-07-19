#version 330 core

out vec4 FragColor;

in vec3 localPos;

uniform sampler2D equirectangularMap;

#define PI 3.14159265359

// Standard maths equation to sample equirectangular map
vec2 SampleSphericalMap(vec3 v)
{
    // Convert to polar
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(1/(2*PI), 1/PI);
    // Normalise
    uv += 0.5;
    return uv;
}

void main()
{		
    vec3 vec = normalize(localPos);
    vec2 uv = SampleSphericalMap(vec);
    vec3 colour = texture(equirectangularMap, uv).rgb;
    
    FragColor = vec4(colour, 1.0);
}
