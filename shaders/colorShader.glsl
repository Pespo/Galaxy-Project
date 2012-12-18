#version 150


// Uniforms : data shared by every shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

#ifdef _VERTEX_

// Attributes : per vertex data
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;

// Varyings : data to transmit to fragments
smooth out vec4 localColor;

void main()
{

    gl_Position = projection * view * model * vec4(vertexPosition,1);
}

#endif




#ifdef _FRAGMENT_


// Final output
out vec4 fragColor;

void main()
{
    fragColor = color;
}

#endif
