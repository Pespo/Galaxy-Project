#version 150

// Uniforms : data shared by every shader
uniform mat4 projection;
uniform mat4 inverseProjection;
uniform vec3 cameraPosition;

uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D Depth;

#ifdef _VERTEX_

in vec4 vertexPosition;
in vec2 vertexUv;

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

vec3 pointLight(in vec3 lcolor, in float intensity, in vec3 lpos, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
	vec3 l =  lpos - fpos;
	vec3 v = fpos - cpos;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(n, l), 0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0, 1.0);
	float d = distance(l, fpos);
	float att = clamp( 1.0 / ( 1 + 0.1 * (d*d)), 0.0, 1.0);
	vec3 color = lcolor * intensity * att * (diffuse * n_dot_l  + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
	return color;
}

vec3 directionalLight(in vec3 lcolor, in float intensity, in vec3 ldir, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
	vec3 l = ldir;
	vec3 v = fpos - cpos;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(n, -l), 0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0, 1.0);
	float d = distance(l, fpos);
	vec3 color = lcolor * intensity * (diffuse * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
	return color;
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
	vec3 diffuse = texture(Diffuse, uv).rgb;
	float spec = texture(Diffuse, uv).a;
	vec3 normal = texture(Normal, uv).rgb;
	float depth = texture(Depth, uv).r * 2.0 -1.0;

	vec2  xy = uv * 2.0 -1.0;
	vec4  wPosition =  vec4(xy, depth, 1.0 ) * inverseProjection;
	vec3  position = vec3(wPosition/wPosition.w);

	vec3 cpointlight1 = pointLight(vec3(0.2, 0.2, 1.0), 10.0, vec3(0.0, 0.0, 0.0), normal, position, diffuse.rgb, spec, cameraPosition);
	//vec3 cpointlight2 = pointLight(vec3(1.0, 0.0, 0.0), 1.0, vec3(10.0, 1.0, 10.0), n, position, diffuse, spec, cameraPosition);
	vec3 cdirlight1 = directionalLight(vec3(0.8, 0.8, 1.0), 1.0, vec3(0.0, -1.0, 0.0), normal, position, diffuse.rgb, spec, cameraPosition);
	//vec3 cspotlight1 = spotLight(vec3(1.0, 1.0, 0.0), 1., vec3(0., -1., 0.), vec3( 0., 5.5, 0.), n, position, diffuse, spec, cameraPosition );

	fragColor = vec4(cpointlight1 + cdirlight1 , 1.0);
	//fragColor = vec4(cdirlight1, 1.0);
}

#endif
