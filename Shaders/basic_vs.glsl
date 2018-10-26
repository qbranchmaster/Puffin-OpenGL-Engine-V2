#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 coords;

out vec2 tex_coords;

void main() {
    tex_coords = coords;
    gl_Position = vec4(position, 1.0);
}