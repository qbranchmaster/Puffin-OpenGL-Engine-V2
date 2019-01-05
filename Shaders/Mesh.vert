#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coord;
layout(location = 2) in vec3 normal_vector;

struct Matrices {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
    mat3 normal_matrix;
};

struct DirectionalLight {
    bool enabled;
    vec3 direction;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct Lighting {
    bool enabled;
    DirectionalLight directional_light;
};

out VS_OUT {
    vec3 position_WORLD;
    vec3 position_VIEW;

    vec2 texture_coord_MODEL;

    vec3 normal_vector_VIEW;

    vec3 directional_light_direction_VIEW;
} vs_out;

uniform Matrices matrices;
uniform Lighting lighting;

void main() {
    vs_out.texture_coord_MODEL = texture_coord;
    vs_out.normal_vector_VIEW = vec3(matrices.view_matrix *
        vec4(normalize(matrices.normal_matrix * normal_vector), 0.0f));

    vs_out.position_WORLD = vec3(matrices.model_matrix * vec4(position, 1.0f));
    vs_out.position_VIEW = vec3(matrices.view_matrix *
        vec4(vs_out.position_WORLD, 1.0f));

    vs_out.directional_light_direction_VIEW =
        normalize(vec3(matrices.view_matrix *
        vec4(lighting.directional_light.direction, 0.0f)));

    gl_Position = matrices.projection_matrix * matrices.view_matrix *
        matrices.model_matrix * vec4(position, 1.0f);
}