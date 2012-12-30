#version 150

// Uniforms : data shared by every shader
uniform mat4 projection;

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

uniform sampler2D Color;
uniform vec2 direction;
// Final output
out vec4 fragColor;

void main () {
    int SampleCount = 9;
    
    float weight = 1.0 / (SampleCount * 2);
    vec3 color = vec3(0.0, 0.0, 0.0);
    for(int i=-SampleCount;i<=SampleCount;++i)
    {
        vec2 coord = floor(gl_FragCoord.xy * (2.0 - direction.y)  ) + i*direction;
        color += texelFetch(Color,ivec2(coord),0).xyz * weight;
        //vec2 coord = uv + (TexelSize * i * Direction);
        //colorTemp += texture(Color, coord).rgb * weight;
    }
    fragColor = vec4(color, 1.0);
}

#endif
