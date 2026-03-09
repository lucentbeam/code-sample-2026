R"====(#version 300 es
layout (location = 0) in vec2 vpos;
layout (location = 1) in vec2 uv;

out vec2 texCoords;

uniform vec2 size;

uniform float scale;

uniform float rotation;

uniform vec2 anchor;

uniform vec2 screenSize;

uniform vec2 center;

uniform int tileID;

uniform vec2 tileSheetSize;

void main()
{
    vec2 loc = (vpos + vec2(0.5,0.5) - anchor);

    loc = vec2(loc.x * cos(rotation) - loc.y * sin(rotation),
               loc.x * sin(rotation) + loc.y * cos(rotation));

    /// scale the vertex coordinate to its correct screen size and offset
    loc = (loc * size * scale + center) * 2.0 / screenSize;

    // shift origin to top left instead of screen center
    loc = loc - vec2(1.0, 1.0);

    // invert y (NDCs y=0 at bottom of screen; top is desired)
    loc = loc * vec2(1.0,-1.0);

    // tilt the top vertices up slightly to get y-sorting from depth test
    mediump float z = gl_VertexID < 2 ? 0.999 : 1.0;

    gl_Position = vec4(loc, z, 1.0);

    mediump vec2 tile_size = vec2(1.0, 1.0) / tileSheetSize;
    mediump vec2 tile_start = vec2(float(tileID % int(tileSheetSize.x)), float(tileID / int(tileSheetSize.x)));
    texCoords = (uv + tile_start) * tile_size;
}
)===="
