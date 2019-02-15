#version 330 core

struct Color {
    samplerCube cube_texture;
    vec3 light_color;
    float gamma;
};

in VS_OUT {
    vec3 texture_coordinates;
} fs_in;

out vec4 frag_color;

uniform Color color;

vec3 gammaCorrection(vec3 input_color) {
    vec3 result = pow(input_color, vec3(color.gamma));
    return result;
}

void main() {
    vec4 cube_texel = texture(color.cube_texture, fs_in.texture_coordinates);
    cube_texel.rgb = gammaCorrection(cube_texel.rgb);

    vec3 texel_color = cube_texel.rgb * color.light_color;
    frag_color = vec4(texel_color, 1.0f);
}