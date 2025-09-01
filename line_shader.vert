
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

uniform vec2 uScreenDimensions;
out vec3 fragColor;

void main()
{
    gl_Position = vec4(aPos.x * (uScreenDimensions.y / uScreenDimensions.x), aPos.y, 0.0, 1.0);
    fragColor = aColor;
}

