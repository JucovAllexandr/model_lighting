#version 330
layout (location = 0) in vec3 mPos;
layout (location = 1) in vec3 mNormal;
layout (location = 2) in vec2 mTexCoords;

uniform  mat4 M;
uniform  mat4 V;
uniform  mat4 P;
in vec3 colorAttr;
out vec3 objectColor;
out vec3 FragPos;
out vec3 Normal;
out vec2 tex_coords;
void main()
{
    FragPos = vec3(M*vec4(mPos,1));
    objectColor = vec3(0.0,0.47,0.77);
    Normal = mat3(transpose(inverse(M)))*mNormal;
    tex_coords = mTexCoords;
    gl_Position =  P*V*M * vec4(mPos,1);
}

