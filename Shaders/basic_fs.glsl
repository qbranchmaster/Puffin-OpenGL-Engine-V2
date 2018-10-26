#version 330 core

uniform sampler2D basic_texture;

in vec2 tex_coords;

out vec4 frag_color;

void main() {
    vec4 texel = texture(basic_texture, tex_coords);
    frag_color = texel;
}