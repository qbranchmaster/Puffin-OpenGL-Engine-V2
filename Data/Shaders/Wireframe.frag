#version 330 core

out vec4 frag_color;

uniform vec3 wireframe_color;

void main() {
    frag_color = vec4(wireframe_color, 1.0f);
}