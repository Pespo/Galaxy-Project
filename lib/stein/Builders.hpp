#ifndef __BUILDERS_HPP__
#define __BUILDERS_HPP__

#include "MeshBuilder.h"

namespace stein {

class Object; //Forward declaration

// Building functions
void buildTriangle(Object &object, MeshBuilder & builder);
void buildSquare(Object &object, const float side, MeshBuilder & builder);
void buildRepere(Object &object, MeshBuilder & builder);
void buildCube(Object &object, const float side, MeshBuilder & builder);
void buildSkybox(Object &object, const float side, MeshBuilder & builder);
void buildSphere(Object &object, const float radius, const size_t discLat, const size_t discLong, MeshBuilder & builder);
bool buildObjectGeometryFromOBJ(Object &object, const char* fileName, bool smoothObject, bool normalize, MeshBuilder & builder);

} // namespace stein

#endif
