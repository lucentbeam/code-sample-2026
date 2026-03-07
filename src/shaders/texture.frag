R"====(#version 300 es

in mediump vec2 texCoords;
out lowp vec4 FragColor;

uniform sampler2D tex0;

uniform lowp vec4 color;

void main()
{
    lowp vec4 c = texture(tex0, texCoords) * vec4(color.xyz, 1.0);
    if (c.a < 0.1) discard;
    c *= color.a;
    FragColor = c;
}
)===="
