#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in float _alpha;

out float alpha;

uniform float Index;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    if(aPos.w == Index){
        gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
        alpha = _alpha;
    }else{
        alpha = 0.0f;
    }
}