#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform vec3 viewPos;
uniform mat4 DepthBiasMVP;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VERT_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 ViewPos;
    vec3 ViewDir;
    vec3 Normal;
    mat3 TBN;
} vert_out;

void main()
{
    // Normal Mapping concepts from https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N); // Gram-Schmidt process to improve quality
    vec3 B = cross(N, T); // Calculate Bitangent
    mat3 TBN = transpose(mat3(T, B, N)); 

    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    // Working in Tangent Space from now on
    // Therefore multiply all matrices by the TBN mat
    vert_out.FragPos = TBN * vec3(model * vec4(aPos, 1.0f));
    vert_out.TexCoords = aTexCoords;
    vert_out.ViewPos = TBN * viewPos;
    vert_out.ViewDir = normalize(TBN * (viewPos - vec3(model * vec4(aPos, 1.0f))));
    vert_out.Normal = normalize(TBN  * aNormal);   
    vert_out.TBN = TBN;
}