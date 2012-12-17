#version 150

// Uniforms : data shared by every shader
uniform mat4 model;
uniform mat4 view;
uniform vec3 cameraPosition;
uniform mat4 projection;
uniform vec4 color;

uniform sampler2D textureUnitDiffuse;

uniform vec3  LightPosition;
//uniform vec3  LightColor;
uniform float LightIntensity;
uniform mat4 InverseViewProjection;

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
	gl_Position = projection * view * model * vertexPosition;
}

#endif

#ifdef _FRAGMENT_

// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;
in vec3 position;
in vec3 normal;

// Final output
out vec4 fragColor;

vec3 pointLight(in vec3 lcolor, in float intensity, in vec3 lpos, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
	vec3 l =  lpos - fpos;
	vec3 v = fpos - cpos;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(n, l), 0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0, 1.0);
	float d = distance(l, position);
	float att = clamp( 1.0 / ( 1 + 0.1 * (d*d)), 0.0, 1.0);
	vec3 color1 = lcolor * intensity * att * (diffuse * n_dot_l  + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
	return color1;
}

vec3 directionalLight(in vec3 lcolor, in float intensity, in vec3 ldir, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
	vec3 l = ldir;
	vec3 v = fpos - cpos;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(n, -l), 0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0, 1.0);
	float d = distance(l, position);
	vec3 color1 = lcolor * intensity * (diffuse * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
	return color1;
}

vec3 spotLight(in vec3 lcolor, in float intensity, in vec3 ldir, in vec3 lpos, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
	vec3 l =  lpos - fpos;
	float cosTs = dot( normalize(-l), normalize(ldir) ); 
	float thetaP =  radians(30.0);
	float cosTp = cos(thetaP);      
	vec3 v = fpos - cpos;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(n, l), 0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0, 1.0);
	float d = distance(l, fpos);
	vec3 color = vec3(0.0, 0.0, 0.0);
	if (cosTs > cosTp) 
		color = pow(cosTs, 30.0) * lcolor * intensity * (diffuse * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
	return color;
}

void main(void)
{
	vec3 diffuse = texture(textureUnitDiffuse, uv).rgb;
	float spec = 0.;
	vec3 n = normalize(normal);

	vec3 cpointlight1 = pointLight(vec3(1.0, 1.0, 0.0), 20.0, vec3(0.0, 0.0, 0.0), n, position, diffuse, spec, cameraPosition);
	//vec3 cpointlight2 = pointLight(vec3(1.0, 0.0, 0.0), 1.0, vec3(10.0, 1.0, 10.0), n, position, diffuse, spec, cameraPosition);
	//vec3 cdirlight1 = directionalLight(vec3(1.0, 0.0, 0.0), 10.0, vec3(0.0, -1.0, 0.0), n, position, diffuse, spec, cameraPosition);
	//vec3 cspotlight1 = spotLight(vec3(1.0, 1.0, 0.0), 1., vec3(0., -1., 0.), vec3( 0., 5.5, 0.), n, position, diffuse, spec, cameraPosition );

	//fragColor = vec4(cpointlight1 + cpointlight2 + cdirlight1 + cspotlight1, 1.0);
	//fragColor = vec4(cpointlight1, 1.0);
	//fragColor = vec4(cpointlight1 + cpointlight2, 1.0);

	fragColor = vec4(diffuse, 1.0);
	//fragColor = vec4(cdirlight1, 1.0);
}

#endif