#version 450
in vec4 in_position;

out vec3 tex_coords;
uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform vec3 eyepos;
void main()
{
    tex_coords = in_position.xyz;
    mat4 new_model = model;
    new_model[3][0] += eyepos.x;
    new_model[3][2] += eyepos.z;
    gl_Position = proj * view *new_model* in_position;
}
