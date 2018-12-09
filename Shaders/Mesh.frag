#version 330 core

in vec3 nor;

out vec4 frag_color;

void main() {
    frag_color = vec4(nor, 1.0f);
}