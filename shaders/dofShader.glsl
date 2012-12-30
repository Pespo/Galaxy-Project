#version 150

// Uniforms : data shared by every shader
uniform mat4 projection;
uniform sampler2D Color;
uniform sampler2D Blur;
uniform sampler2D BlurCoef;

#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec2 vertexUv;
// Varyings : data to transmit to fragments
out vec2 uv;

void main(void)
{
    uv = vertexUv;
    gl_Position = projection * vertexPosition;
}

#endif

#ifdef _FRAGMENT_

// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;

// Final output
out vec4 fragColor;

void main(void)
{ 
    float blurCoef = texture(BlurCoef, uv).r;
    fragColor = vec4(mix(texture(Color, uv).rgb, texture(Blur, uv).rgb, blurCoef), 1.0);
}

#endif
