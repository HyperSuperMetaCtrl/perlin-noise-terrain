#version 450

in vec4 in_position;
in vec2 in_texCoords;
in vec3 in_normals;
out vec2 texCoords;
out vec3 normals;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 nmodel;
void main()
{
    normals = nmodel*in_normals;
    texCoords = in_texCoords;
    vec4 new_pos = in_position;
    if (new_pos.y < 0.142){
        new_pos.y=0.142;
        normals.xyz = vec3(0,1,0);
    }
    gl_Position =proj*view  * model*new_pos;
}

