#version 330 core

struct Material {
    bool has_diffuse_texture;
    bool has_normalmap_texture;
    sampler2D diffuse_texture;
    sampler2D normalmap_texture;
    vec3 ka;
    vec3 kd;
    vec3 ks;
    int shininess;
    float reflectivity;
};

in vec2 tex_coord;

out vec4 frag_color;

uniform Material object_material;

void main() {
    vec4 texel = texture(object_material.diffuse_texture, tex_coord);
    frag_color = texel;
}