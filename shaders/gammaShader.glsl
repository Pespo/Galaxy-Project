#version 150

// Uniforms : data shared by every shader
uniform mat4 projection;
uniform sampler2D Color;

#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec2 vertexUv;
// Varyings : data to transmit to fragments
out vec2 uv;

void main(void)
{	
	uv = vertexUv.xy;
	gl_Position = projection * vertexPosition;
}

#endif

#ifdef _FRAGMENT_

out vec4  fragColor;

uniform mat3 G[2] = mat3[](
        mat3( 1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0 ),
        mat3( 1.0, 0.0, -1.0, 2.0, 0.0, -2.0, 1.0, 0.0, -1.0 )
);


void main(void)
{
    float Gamma = 1.0;
    float SobelCoef = 0.5;
	//vec3 color = texture(Color, uv).rgb;
	vec3 color = texelFetch(Color, ivec2(gl_FragCoord.xy), 0).rgb;

	mat3 I;
    for (int i=0; i<3; i++)
    for (int j=0; j<3; j++) {
            vec3 sample  = texelFetch( Color, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
            I[i][j] = length(sample); 
    }

    /* calculate the convolution values for all the masks */                                                              
	float cnv[2];
    for (int i=0; i<2; i++) {                              
            float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]);                                     
            cnv[i] = dp3;                            
    }   

    float c =  sqrt(cnv[0]*cnv[0]+cnv[1]*cnv[1]);
    //OutColor = vec4( c, c, c, 1.0);
	//OutColor = vec4(pow(color, vec3(1.0/Gamma)), 1.0);
	fragColor = vec4(pow(color, vec3(1.0/Gamma))- SobelCoef * 0.5 * c, 1.0);

}

#endif
