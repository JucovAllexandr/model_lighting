#version 330
layout (location = 0) in vec3 mPos;
layout (location = 1) in vec3 mNormal;
layout (location = 2) in vec2 mTexCoords;

uniform  mat4 M;
uniform  mat4 V;
uniform  mat4 P;
in vec3 colorAttr;
out vec4 color;

void main()
{
    gl_Position =  P*V*M * vec4(mPos,1);
    color = vec4(colorAttr,1);
}

