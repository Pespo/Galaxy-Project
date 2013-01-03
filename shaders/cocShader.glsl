#version 150

// Uniforms : data shared by every shader
uniform mat4 projection;
uniform mat4 inverseProjection;

#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec2 vertexUv;
// Varyings : data to transmit to fragments
out vec2 uv;

void main()
{
    uv = vertexUv;
    gl_Position = projection * vertexPosition;
}

#endif

#ifdef _FRAGMENT_

// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;

uniform sampler2D Depth;
// Final output
out vec4 fragColor;

void main() {
    float depth = texture(Depth, uv).r;
    vec3 Focus = vec3(0.5 , 1., 10.);
    vec2  xy = uv * 2.0 -1.0;
    vec4  wViewPos =  vec4(xy, depth * 2.0 -1.0, 1.0) * inverseProjection;
    vec3  viewPos = vec3(wViewPos/wViewPos.w);
    float viewDepth = -viewPos.z;
    float coef;
    if( viewDepth < Focus.x )
        fragColor = vec4( clamp( abs( (viewDepth - Focus.x) / Focus.y ), 0.0, 1.0), 0.0, 0.0, 1.0 );
    else
        fragColor = vec4( clamp( abs( (viewDepth - Focus.x) / Focus.z ), 0.0, 1.0), 0.0, 0.0, 1.0 );
}

#endif
