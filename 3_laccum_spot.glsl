#if defined(VERTEX)
uniform mat4 Projection;

in vec3 VertexPosition;
in vec3 VertexTexCoord;

out vec2 uv;

void main(void)
{	
	uv = VertexTexCoord.xy;
	gl_Position = Projection * vec4(VertexPosition, 1.0);
}

#endif

#if defined(FRAGMENT)

in vec2 uv;

uniform sampler2D Material;
uniform sampler2D Normal;
uniform sampler2D Depth;
uniform sampler2D Shadow;

uniform vec3  CameraPosition;
uniform vec3  LightPosition;
uniform vec3  LightDirection;
uniform vec3  LightColor;
uniform float LightIntensity;
uniform mat4  InverseViewProjection;
uniform mat4  ProjectionLightBias;
uniform float ShadowBias;
uniform float Time;

out vec4  Color;

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
	vec4  material = texture(Material, uv).rgba;
	vec3  normal = texture(Normal, uv).rgb;
	float depth = texture(Depth, uv).r * 2.0 -1.0;

	vec2  xy = uv * 2.0 - 1.0;
	vec4  wPosition =  vec4(xy, depth, 1.0 ) * InverseViewProjection;
	vec3  position = vec3(wPosition/wPosition.w);

	vec3 diffuse = material.rgb;
	float spec = material.a;

	vec3 n = normalize(normal);

	vec3 cspotlight1 = spotLight(LightColor, LightIntensity, LightDirection, LightPosition, n, position, diffuse, spec, CameraPosition );
	
	vec4 wlightSpacePosition = ProjectionLightBias * vec4(position, 1.0);
	//if (wlightSpacePosition.w > 0.0  && lightSpacePosition.x > 0.0 && lightSpacePosition.x < 1.0 && lightSpacePosition.y > 0.0 && lightSpacePosition.y < 1.0 )
		vec3 lightSpacePosition = vec3(wlightSpacePosition/wlightSpacePosition.w);
	
	float shadowMap = texture(Shadow, uv).r;	
	float shadowMapProjection = texture(Shadow, lightSpacePosition.xy).r;

	
	if((shadowMapProjection - lightSpacePosition.z ) + ShadowBias <= 0)
		Color = vec4(0, 0, 0, 1);
	else
		Color = vec4(cspotlight1, 1.0);
}

#endif
