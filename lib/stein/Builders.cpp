#include "Builders.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Tools.hpp"
#include "MeshBuilder.h"
#include "math/StreamUtils.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cfloat>
#define M_PI 3.14

using namespace std;

namespace stein {

// Builds one triangle
void buildTriangle(Object &object, MeshBuilder & builder) {
    builder.addVertex(-0.2, 0, 0);
    builder.addVertex(0.2, 0, 0);
    builder.addVertex(0, 1, 0);
    builder.addNormal(0, 0, 1);
    builder.addFace(0, 1, 2).setNormalIndices(0, 0, 0);
    
    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendPrimitives(vertices, indices);
}

// Builds one square
void buildSquare(Object &object, const GLfloat side, MeshBuilder & builder) {
    builder.addVertex(-side/2, -side/2, 0); // bottom left
    builder.addVertex(side/2, -side/2, 0); // bottom right
    builder.addVertex(-side/2, side/2, 0); // top left
    builder.addVertex(side/2, side/2, 0); // top right

    builder.addNormal(0, 0, 1);
    
    builder.addUV(0., 0.);
    builder.addUV(1., 0.);
    builder.addUV(0., 1.);
    builder.addUV(1., 1.);

    MeshBuilder::Face &triangle1 = builder.addFace(0, 1, 2);
        triangle1.setNormalIndices(0, 0, 0);
        triangle1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &triangle2 = builder.addFace(2, 1, 3);
        triangle2.setNormalIndices(0, 0, 0);
        triangle2.setUvIndices(2, 1, 3);

    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendUvs(uvs);
    object.sendNormals(normals);
    object.sendPrimitives(vertices, indices);
}

void buildRepere(Object &object, MeshBuilder & builder)
{
    // The 3 vertices of a triangle
    builder.addVertex(  0.0, 0.0, 0.0);
    builder.addVertex(  1.0, 0.0, 0.0);
    builder.addVertex(  0.0, 1.0, 0.0);
    builder.addVertex(  0.0, 0.0, 1.0);


    builder.addFace(0, 1, 2);
    builder.addFace(0, 1, 3);
    builder.addFace(0, 2, 3);
    
    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendPrimitives(vertices, indices);

}

// Builds one Cube
void buildCube(Object &object, const GLfloat side, MeshBuilder & builder) {
    builder.addVertex(-side, -side, side); // 0 - front bottom left
    builder.addVertex(side, -side, side); // 1 - front bottom right
    builder.addVertex(-side, side, side); // 2 - front top left
    builder.addVertex(side, side, side); // 3 - front top right
    builder.addVertex(-side, -side, -side); // 4 - back bottom left
    builder.addVertex(side, -side, -side); // 5 - back bottom right
    builder.addVertex(-side, side, -side); // 6 - back top left
    builder.addVertex(side, side, -side); // 7 - back top right

    builder.addNormal(-1, 0, 0);
    builder.addFace(0, 4, 2).setNormalIndices(0, 0, 0); 
    builder.addFace(2, 4, 6).setNormalIndices(0, 0, 0); // left

    builder.addNormal(1, 0, 0);
    builder.addFace(1, 5, 3).setNormalIndices(1, 1, 1);
    builder.addFace(3, 5, 7).setNormalIndices(1, 1, 1); // right

    builder.addNormal(0, 1, 0);
    builder.addFace(2, 3, 6).setNormalIndices(2, 2, 2);
    builder.addFace(6, 3, 7).setNormalIndices(2, 2, 2); // top

    builder.addNormal(0, -1, 0);
    builder.addFace(0, 4, 1).setNormalIndices(3, 3, 3);
    builder.addFace(1, 4, 5).setNormalIndices(3, 3, 3); // bottom

    builder.addNormal(0, 0, -1);
    builder.addFace(4, 6, 5).setNormalIndices(4, 4, 4);
    builder.addFace(5, 6, 7).setNormalIndices(4, 4, 4); // back

    builder.addNormal(0, 0, 1);
    builder.addFace(0, 1, 2).setNormalIndices(5, 5, 5);
    builder.addFace(2, 1, 3).setNormalIndices(5, 5, 5); // front
    
    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendNormals(normals);
    object.sendPrimitives(vertices, indices);
}

void buildSkybox(Object &object, const GLfloat side, MeshBuilder & builder) {
    builder.addVertex(-side, -side, side); // 0 - front bottom left
    builder.addVertex(side, -side, side); // 1 - front bottom right
    builder.addVertex(-side, side, side); // 2 - front top left
    builder.addVertex(side, side, side); // 3 - front top right
    builder.addVertex(-side, -side, -side); // 4 - back bottom left
    builder.addVertex(side, -side, -side); // 5 - back bottom right
    builder.addVertex(-side, side, -side); // 6 - back top left
    builder.addVertex(side, side, -side); // 7 - back top right

    builder.addNormal(1, 0, 0);
    builder.addNormal(-1, 0, 0);
    builder.addNormal(0, -1, 0);
    builder.addNormal(0, 1, 0);
    builder.addNormal(0, 0, 1);
    builder.addNormal(0, 0, -1);

    builder.addUV(0., 0.);
    builder.addUV(0., 1.);
    builder.addUV(1., 0.);
    builder.addUV(1., 1.);

    // left
    MeshBuilder::Face &left1 = builder.addFace(0, 2, 4);  
        left1.setNormalIndices(0, 0, 0);
        left1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &left2 = builder.addFace(4, 2, 6);
        left2.setNormalIndices(0, 0, 0);
        left2.setUvIndices(2, 1, 3);
    
    // right
    MeshBuilder::Face &right1 = builder.addFace(1, 3, 5);
        right1.setNormalIndices(1, 1, 1);
        right1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &right2 = builder.addFace(5, 3, 7);
        right2.setNormalIndices(1, 1, 1); 
        right2.setUvIndices(2, 1, 3);
    
    // top
    MeshBuilder::Face &top1 = builder.addFace(2, 6, 3);
        top1.setNormalIndices(2, 2, 2);
        top1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &top2 = builder.addFace(3, 6, 7);
        top2.setNormalIndices(2, 2, 2);
        top2.setUvIndices(2, 1, 3);

    // bottom
    MeshBuilder::Face &bottom1 = builder.addFace(0, 1, 4);
        bottom1.setNormalIndices(3, 3, 3);
        bottom1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &bottom2 = builder.addFace(4, 1, 5);
        bottom2.setNormalIndices(3, 3, 3); 
        bottom2.setUvIndices(2, 1, 3);

    // back
    MeshBuilder::Face &back1 = builder.addFace(4, 5, 6);
        back1.setNormalIndices(4, 4, 4);
        back1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &back2 = builder.addFace(6, 5, 7);
        back2.setNormalIndices(4, 4, 4); 
        back2.setUvIndices(2, 1, 3);

    // front
    MeshBuilder::Face &front1 = builder.addFace(0, 2, 1);
        front1.setNormalIndices(5, 5, 5);
        front1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &front2 = builder.addFace(1, 2, 3);
        front2.setNormalIndices(5, 5, 5); 
        front2.setUvIndices(2, 1, 3);
    
    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendUvs(uvs);
    object.sendNormals(normals);
    object.sendPrimitives(vertices, indices);

}

void buildSphere(Object &object, const float radius, size_t discLat, size_t discLong, MeshBuilder & builder){ 

    //object->nbVertices = (90 / discLat) * (360 / discLong) * 6 *2; 
    //object->nbIndices  = (90 / discLat) * (360 / discLong) * 6 *2; 
    
    //GLfloat deltaLat=(M_PI/2.0)/20; 
    //GLfloat deltaLong=(2*M_PI)/discAngle; 
        
    //GLfloat vertices[object->nbVertices*4]; 
    //GLuint indices[object->nbIndices*4]; 
    //GLfloat normals[object->nbVertices*4]; 
    GLuint nbVertices = (90 / discLat) * (360 / discLong) * 6 *2;
    //GLuint indexVertices = 0; 
    //GLdouble a; 
    GLdouble b; 
      
    GLdouble aLong; 
    //GLdouble bLat; 
  
    // north hemisphere 
    for( b = 0; b <= 90 - discLat; b+=discLat){ 
              
        for( aLong = 0; aLong <= 360 - discLong; aLong+=discLong){ 
            
             //first triangle 
            builder.addVertex(  radius * sin((aLong) / 180 * M_PI) * sin((b) / 180 * M_PI), //– H 
                                radius * cos((b) / 180 * M_PI), //– Z 
                                radius * cos((aLong) / 180 * M_PI) * sin((b) / 180 * M_PI) ) ; //+ K 

            builder.addVertex(  radius * sin((aLong + discLong) / 180 * M_PI) * sin((b) / 180 * M_PI), //– H 
                                radius * cos((b) / 180 * M_PI), //– Z 
                                radius * cos((aLong + discLong) / 180 * M_PI) * sin((b ) / 180 * M_PI) ); //+ K 
              
            builder.addVertex(  radius * sin((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI), //– H 
                                radius * cos((b + discLat) / 180 * M_PI), //– Z 
                                radius * cos((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI) ); //+ K 
              
            //second triangle 
            builder.addVertex(  radius * sin((aLong + discLong) / 180 * M_PI) * sin((b) / 180 * M_PI), //– H 
                                radius * cos((b) / 180 * M_PI), //– Z 
                                radius * cos((aLong + discLong) / 180 * M_PI) * sin((b ) / 180 * M_PI) ); //+ K               
              
            builder.addVertex(  radius * sin((aLong + discLong) / 180 * M_PI) * sin((b+ discLat) / 180 * M_PI), 
                                radius * cos((b+ discLat) / 180 * M_PI),  
                                radius * cos((aLong + discLong) / 180 * M_PI) * sin((b+ discLat) / 180 * M_PI) ); 
    
            builder.addVertex(  radius * sin((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI), //– H 
                                radius * cos((b + discLat) / 180 * M_PI), //– Z 
                                radius * cos((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI) ); //+ K 
        } 
    } 
      
    // south hemisphere      
    for( b = 0; b <= 90 - discLat; b+=discLat) 
    { 
        for( aLong = 0; aLong <= 360 - discLong; aLong+=discLong) 
        { 
            //first triangle            
            builder.addVertex(  radius * sin((aLong) / 180 * M_PI) * sin((b) / 180 * M_PI), //– H 
                                - radius * cos((b) / 180 * M_PI), //– Z 
                                radius * cos((aLong) / 180 * M_PI) * sin((b) / 180 * M_PI) ); //+ K 

            builder.addVertex(  radius * sin((aLong + discLong) / 180 * M_PI) * sin((b) / 180 * M_PI), //– H 
                                - radius * cos((b) / 180 * M_PI), //– Z 
                                radius * cos((aLong + discLong) / 180 * M_PI) * sin((b ) / 180 * M_PI) ); //+ K 
                                
              
            builder.addVertex(  radius * sin((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI), //– H 
                                - radius * cos((b + discLat) / 180 * M_PI), //– Z 
                                radius * cos((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI) ); //+ K 

            //second triangle 
            builder.addVertex(  radius * sin((aLong + discLong) / 180 * M_PI) * sin((b) / 180 * M_PI), //– H 
                                - radius * cos((b) / 180 * M_PI), //– Z 
                                radius * cos((aLong + discLong) / 180 * M_PI) * sin((b ) / 180 * M_PI) ); //+ K 
                                
            builder.addVertex(  radius * sin((aLong + discLong) / 180 * M_PI) * sin((b+ discLat) / 180 * M_PI), 
                                - radius * cos((b+ discLat) / 180 * M_PI),  
                                radius * cos((aLong + discLong) / 180 * M_PI) * sin((b+ discLat) / 180 * M_PI) ); 
                                
            builder.addVertex(  radius * sin((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI), //– H 
                                - radius * cos((b + discLat) / 180 * M_PI), //– Z 
                                radius * cos((aLong) / 180 * M_PI) * sin((b + discLat) / 180 * M_PI) ); //+ K 
        } 
    } 

    for(int k=0; k<nbVertices; k+=3) 
    { 
        builder.addFace(k, k+1, k+2); 
    } 
      
    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendPrimitives(vertices, indices);  
} 
  

struct Triple {
    GLuint a, b, c;
};

static Triple readTriple(string& line) {
    replace(line.begin(), line.end(), '/', ' ');
    istringstream ss(line);
    size_t a, b, c;
    ss >> a >> b >> c;
    Triple value = { --a, --b, --c };
    return value;
}

// Builds an object made from an OBJ file, taking only geometry into account (not materials)
bool buildObjectGeometryFromOBJ(Object &object, const char* fileName, bool smoothObject, bool normalize, MeshBuilder & builder) {
    ifstream file(fileName, ios_base::in);
    if (!file) {
        cerr << "       Error while loading object from .obj file : " << fileName << "." << endl;
        return false;
    }

    bool hasVt = false;
    bool hasVn = false;
    string buf, key, name, MTLFileName;

    while (getline(file, buf)) {
        istringstream line(buf);
        line >> key;
        if (key == "o")
            line >> name;
        else if (key == "v") {
            float x, y, z;
            line >> x >> y >> z;
            builder.addVertex(x, y, z);
        } else if (key == "vt") {
            float u, v;
            line >> u >> v;
            builder.addUV(u, 1 - v);
            hasVt = true;
        } else if (key == "vn") {
            float x, y, z;
            line >> x >> y >> z;
            builder.addNormal(x, y, z);
            hasVn = true;
        } else if (key == "f") {
            string a, b, c;
            line >> a >> b >> c;
            const Triple t1 = readTriple(a);
            const Triple t2 = readTriple(b);
            const Triple t3 = readTriple(c);
            builder.addFace(t1.a, t2.a, t3.a).setUvIndices(t1.b, t2.b, t3.b).setNormalIndices(t1.c, t2.c, t3.c);
        } else if (key == "mltlib")
            line >> MTLFileName;
    }
    
//    if (smoothObject)
//        reorderUvsAndNormalsIfSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
//    else
//        reorderUvsAndNormalsIfNonSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
//    conformToObject(&vertices, &uvs, &normals);
	
	if(normalize) builder.centerAndNormalizeMesh();
    builder.normalizeNormals();

    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    object.sendPrimitives(vertices, indices);

    if (hasVn) {
        const bool oneNormalPerTriangle = (normals.size() == indices.size());
        if (oneNormalPerTriangle)
            if (smoothObject)
                cerr << "       WARNING : smoothObject==true. Normals will be wrong : change it to false." << endl;
        object.sendNormals(normals);
    } else
        cerr << "       WARNING : Obj file " << name << " has no normals, add some in modeler." << endl;

    if (hasVt)
        object.sendUvs(uvs);
    else
        cerr << "       WARNING : Obj file " << name << " has no texture coordinates, add some in modeler." << endl;

    return true;
}

} // namespace stein
