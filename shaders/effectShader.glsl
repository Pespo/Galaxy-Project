#version 150

// Uniforms : data shared by every shader
uniform mat4 projection;
uniform mat4 inverseProjection;

#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec2 vertexUvs;
// Varyings : data to transmit to fragments
out vec2 uv;

void main()
{
    uv = vertexUvs;
    gl_Position = projection * vertexPosition;
}

#endif

#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;

uniform sampler2D Depth;
uniform sampler2D Color;
// Final output
out vec4 fragColor;

vec3 setBlur() {
    int SampleCount = 9;
    vec2 Direction = vec2(1., 0.);
    float weight = 1.0 / (SampleCount * 2);
    vec3 colorTemp = vec3(0.0, 0.0, 0.0);
    for(int i=-SampleCount;i<=SampleCount;++i)
    {
        vec2 coord = floor(gl_FragCoord.xy * (2.0 - Direction.y)  ) + i*Direction;
        colorTemp += texelFetch(Color,ivec2(coord),0).xyz * weight;
        //vec2 coord = uv + (TexelSize * i * Direction);
        //colorTemp += texture(Color, coord).rgb * weight;
    }
    return colorTemp;
}

float setBlurCoef() {
    float depth = texture(Depth, uv).r;
    vec3 Focus = vec3(5., 1., 50.);
    vec2  xy = uv * 2.0 -1.0;
    vec4  wViewPos =  vec4(xy, depth * 2.0 -1.0, 1.0) * inverseProjection;
    vec3  viewPos = vec3(wViewPos/wViewPos.w);
    float viewDepth = -viewPos.z;
    float coef;
    if( viewDepth < Focus.x ){
        coef = clamp( abs( (viewDepth - Focus.x) / Focus.y ), 0.0, 1.0);
    }
    else
        coef = clamp( abs( (viewDepth - Focus.x) / Focus.z ), 0.0, 1.0);

    return coef;
}

void main()
{ 
    
    fragColor = vec4(mix(texture(Color, uv).rgb, setBlur(), setBlurCoef()), 1.0);
}

#endif
