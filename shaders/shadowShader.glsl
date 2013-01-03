#version 150

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Object;
uniform sampler2D Normal;

#ifdef _VERTEX_

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUv;

out vec2 uv;
out vec3 normal;
out vec3 position;

void main(void)
{	
	uv = vertexUv;
	normal = vec3(Object * vec4(texture(Normal, uv).rgb, 1.0));
	position = vec3(Object * vertexPosition);
	gl_Position = Projection * View * vec4(position, 1.0);
}

#endif

#ifdef _FRAGMENT_

out vec4  FragColor;

in vec3 normal;

void main(void)
{
}

#endif
