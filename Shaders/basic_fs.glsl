#version 330 core

in vec3 vertex_color;

out vec4 frag_color;

void main() {
    frag_color = vec4(vertex_color, 1.0f);
}