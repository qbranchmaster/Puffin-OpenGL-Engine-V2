#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coord;
layout(location = 2) in vec3 normal_vector;

struct Matrices {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
};

out vec2 tex_coord;

uniform Matrices matrices;

void main() {
    tex_coord = texture_coord;

    gl_Position = matrices.projection_matrix * matrices.view_matrix *
        matrices.model_matrix * vec4(position, 1.0f);
}