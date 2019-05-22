#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coord;
layout(location = 2) in vec3 normal_vector;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

struct Matrices {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
    mat3 normal_matrix;
    mat4 dir_light_matrix;
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
    bool blinn_phong;
    DirectionalLight directional_light;
    float emission_factor;
};

out VS_OUT {
    vec3 position_WORLD;
    vec3 position_VIEW;
    vec3 position_TANGENT;

    vec3 view_position_TANGENT;

    vec2 texture_coord_MODEL;

    vec3 normal_vector_VIEW;

    vec3 directional_light_direction_VIEW;
    vec3 directional_light_direction_TANGENT;

    vec4 frag_pos_DIR_LIGHT;
} vs_out;

uniform Matrices matrices;
uniform Lighting lighting;

uniform vec4 clipping_plane;

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

    vs_out.frag_pos_DIR_LIGHT = matrices.dir_light_matrix *
        vec4(vs_out.position_WORLD, 1.0f);

    // Calculation for tangent space
    vec3 camera_pos_WORLD = (inverse(matrices.view_matrix) *
        vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;

    // TODO: No need to calculate b? Use this from layout 4
    // TODO: Check if object use normalmap, do not calculate it always
    vec3 t = normalize(matrices.normal_matrix * tangent);
    vec3 n = normalize(matrices.normal_matrix * normal_vector);
    t = normalize(t - dot(t, n) * n);
    vec3 b = cross(n, t);

    mat3 tbn_matrix = transpose(mat3(t, b, n));

    vs_out.position_TANGENT = tbn_matrix * vs_out.position_WORLD;
    vs_out.view_position_TANGENT = tbn_matrix * camera_pos_WORLD;
    vs_out.directional_light_direction_TANGENT = normalize(tbn_matrix *
        lighting.directional_light.direction);

	// Find vertex distance from the plane
	gl_ClipDistance[0] = dot(vec4(vs_out.position_WORLD, 1.0f), clipping_plane);

    gl_Position = matrices.projection_matrix * matrices.view_matrix *
        matrices.model_matrix * vec4(position, 1.0f);
}