#version 330 core

struct Color {
    samplerCube cube_texture;
};

in VS_OUT {
    vec3 texture_coordinates;
} fs_in;

out vec4 frag_color;

uniform Color color;

void main() {
    vec4 cube_texel = texture(color.cube_texture, fs_in.texture_coordinates);
    vec3 result_color = cube_texel.rgb;

    frag_color = vec4(result_color, 1.0f);
}