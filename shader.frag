
#version 330 core
in vec3 fragColor;
out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord * 2.0 - 1.0; // [-1,1] range
    float dist = length(coord);
    if (dist > 1.0) discard;
    FragColor = vec4(fragColor, 1.0);
}

