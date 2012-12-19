#version 150

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float ka;
    float kd;
    float ks;
    float shininess;
};

// Uniforms : data shared by every shader
uniform mat4 model;
uniform mat4 view;
uniform vec4 cameraPosition;
uniform mat4 projection;
uniform vec4 color;

//uniform Light light;
uniform uint material;
uniform bvec4 filledData; // filledData[0] : true if position, 
                          // filledData[1] : true if normals, 
                          // filledData[2] : true if uvs,
                          // filledData[3] : true if colors.

#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUvs;
in vec4 vertexColor;

// Varyings : data to transmit to fragments
smooth out vec4 position;
smooth out vec4 normal;
smooth out vec2 uvs;
smooth out vec4 localColor;

void main()
{
    if (filledData[0]) position = model * vertexPosition;
    if (filledData[1]) normal = model * vec4(vertexNormal, 0.0);
    if (filledData[2]) uvs = vertexUvs;
    if (filledData[3]) localColor = vertexColor;

    gl_Position = projection * view * model * vertexPosition;
}

#endif

#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders
smooth in vec4 position;
smooth in vec4 normal;
smooth in vec2 uvs;
smooth in vec4 localColor;

// Final output
out vec4 fragColor;

Material setMaterial() {
    if(material == uint(0))
        return Material(vec4(0.135, 0.2225, 0.1575, 1.0), vec4(0.54, 0.89, 0.63, 1.0), vec4(0.316228, 0.316228, 0.316228, 1.0), 0.01, 1.0, 2.0, 0.1);
    else if(material == uint(1))
        return Material(vec4(0.3, 0.3, 0.3, 1.0), vec4(0.4, 0.4, 0.4, 1.0), vec4(0., 0., 0., 1.0), 0.01, 1.0, 2.0, 0.6);
    else if(material == uint(2))
        return Material(vec4(0.8, 0.01175, 0.01175, 1.0), vec4(0.8, 0.01, 0.01, 1.0), vec4(1.0, 0.1, 0.1, 1.0), 0.8, 1.0, 2.0, 1.0);
    else if(material == uint(3))
        return Material(vec4(0.05, 0.2, 0.9, 1.0), vec4(0.1, 0.1, 0.9, 1.0), vec4(0.1, 0.1, 0.9, 1.0), 0.8, 1.0, 2.0, 1.2);
    else
        return Material(vec4(0.8, 0.8, 0.1, 1.0), vec4(0.8, 0.8, 0.01, 1.0), vec4(0.8, 0.8, 0.1, 1.0), 0.8, 1.0, 2.0, 1.2);
}

void main()
{   
    Material mat = setMaterial();
    vec4 lposition = vec4 (0., 0., 0., 1.);
    float lpower = 0.5;
    vec4 diffuseColorMix=vec4(mat.diffuse);
    // If color
    if (filledData[3]) diffuseColorMix=mix(localColor, mat.diffuse, 0.3);
    
    // If no normal
    if (!filledData[1]) fragColor = diffuseColorMix;
    else
    {
        vec4 L=normalize(lposition); // Direction of light from fragment -> light.position[3]==0.0 : Directional light
        if (lposition.w==1.0) L=normalize(lposition-position); //   -> light.position[3]==1.0 : Point light
        vec4 V=normalize(cameraPosition-position); // Direction from fragment to cameraPosition
        vec4 R=normalize(reflect(-L, normal)); // Direction of reflected light beam, from fragment
        vec4 N=normalize(normal); // Normal
    
        float ambientValue=lpower;
        float diffuseValue=lpower * max( dot(L, N), 0.0);
        float specularValue=lpower * pow( max(dot(R, V), 0.0), mat.shininess*128);
        vec4 ambientContribution=vec4(ambientValue*mat.ka*mat.ambient.rgb, mat.ambient.a);
        vec4 diffuseContribution=vec4(diffuseValue*mat.kd*diffuseColorMix.rgb, diffuseColorMix.a);
        vec4 specularContribution=vec4(specularValue*mat.ks*mat.specular.rgb, mat.specular.a);
  
        fragColor = ambientContribution + diffuseContribution + specularContribution;
    }
}

#endif
