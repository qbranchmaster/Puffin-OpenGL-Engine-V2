#version 330 core

struct Color {
    samplerCube cube_texture;
};

struct Lighting {
    bool enabled;

    bool ambient_enabled;
    vec3 ambient_color;
};

in VS_OUT {
    vec3 texture_coordinates;
} fs_in;

out vec4 frag_color;

uniform Color color;
uniform Lighting lighting;

vec3 calculateLighting(vec3 input_color) {
    if (!lighting.enabled) {
        return input_color;
    }

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    if (lighting.ambient_enabled) {
        result = input_color * lighting.ambient_color;
    }

    return result;
}

void main() {
    vec4 cube_texel = texture(color.cube_texture, fs_in.texture_coordinates);
    vec3 texel_color = cube_texel.rgb;

    vec3 result_color = calculateLighting(texel_color);

    frag_color = vec4(result_color, 1.0f);
}