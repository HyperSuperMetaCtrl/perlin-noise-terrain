#version 450
out vec4 out_color;
in vec2 texCoords;
in vec3 normals;
uniform sampler2D myTex;
void main()
{
    vec4 sun_pos = vec4(1, 2, 1,0);
    float ambient = 0.1;
    float diffuse = 0.7*max(0,dot(vec4(normalize(normals),1),normalize(sun_pos)));
    out_color = (ambient+diffuse)*texture(myTex,texCoords);
}
