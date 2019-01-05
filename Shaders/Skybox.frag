#version 330 core

struct Color {
    samplerCube cube_texture;
    vec3 light_color;
};

in VS_OUT {
    vec3 texture_coordinates;
} fs_in;

out vec4 frag_color;

uniform Color color;

void main() {
    vec4 cube_texel = texture(color.cube_texture, fs_in.texture_coordinates);
    vec3 texel_color = cube_texel.rgb * color.light_color;
    frag_color = vec4(texel_color, 1.0f);
}