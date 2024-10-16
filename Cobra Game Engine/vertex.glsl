#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 transform;

uniform mat4 view;
uniform mat4 projection; 

out vec2 o_texCoord;
out vec3 o_normal;
out vec3 o_fragPos;
void main()
{
    gl_Position = projection * view * transform * vec4(position, 1.0);
    o_fragPos = vec3(transform * vec4(position, 1.0));
    o_texCoord = texCoord;
    o_normal = normal;
}