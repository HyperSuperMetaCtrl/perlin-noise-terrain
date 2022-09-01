#version 450
out vec4 out_color;

in vec3 tex_coords;

uniform samplerCube myTex;

void main()
{
    out_color = texture(myTex, tex_coords);
}