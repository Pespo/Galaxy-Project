#version 150

// Uniforms : data shared by every shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D textureUnitDiffuse;

#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec2 vertexUv;

// Varyings : data to transmit to fragments
out vec2 uv;
out vec3 normal;
out vec3 position;

void main(void)
{	
	uv = vertexUv;
	gl_Position = projection * view * model * vertexPosition;
}

#endif

#ifdef _FRAGMENT_

// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;

// Final output
out vec4 fragColor;

void main(void)
{
	fragColor = vec4(texture(textureUnitDiffuse, uv).rgb * 0.4, 1.);
}

#endif