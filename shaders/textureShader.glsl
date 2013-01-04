#version 150

// Uniforms : data shared by every shader
uniform mat4 model;
uniform mat4 view;
uniform vec3 cameraPosition;
uniform mat4 projection;

uniform sampler2D textureUnitDiffuse;
uniform sampler2D textureUnitSpecular;
uniform sampler2D textureUnitNormal;
uniform sampler2D textureUnitDisplacement;

#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexUv;

// Varyings : data to transmit to fragments
out vec2 uv;
out vec3 normal;
out vec3 position;

void main(void)
{	
	uv = vertexUv;
	normal = vec3(model * vec4(vertexNormal, 1.0));
	position = vec3(model * vertexPosition);
	// gl_Position = projection * view * model * vertexPosition;

	vec3 dv = texture( textureUnitDisplacement, vertexUv ).xyz;
	float dfx = 0.02 * dv.x;
	vec3 displacedPosition = normal.x * dfx + vertexPosition.xyz;

	position = vec3(model * vec4(displacedPosition, 1.));
	gl_Position = projection * view * model * vec4( displacedPosition, 1.0 );
}

#endif

#ifdef _FRAGMENT_

// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;
in vec3 position;
in vec3 normal;

// Final output
out vec4 fragColor;
out vec4 fragNormal;



void main(void)
{
	vec3 diffuse = texture(textureUnitDiffuse, uv).rgb;
	float spec = texture(textureUnitSpecular, uv).r;
	vec3 n = normalize(texture(textureUnitNormal, uv).rgb);

	fragColor = vec4(normal, spec);
	fragNormal = vec4(n, spec);

}

#endif