#version 330 core

uniform sampler2D basic_texture;
uniform vec3 color_filter;

in vec2 tex_coords;

out vec4 frag_color;

void main() {
    vec4 texel = texture(basic_texture, tex_coords) * vec4(color_filter, 1.0);
    frag_color = texel;
}