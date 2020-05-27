#include "triangle.h"

Triangle::Triangle() {
    Vertex[0] << 0,0,0,1;
    Vertex[1] << 0,0,0,1;
    Vertex[2] << 0,0,0,1;

    Color[0] << 0.0, 0.0, 0.0;
    Color[1] << 0.0, 0.0, 0.0;
    Color[2] << 0.0, 0.0, 0.0;

    TexCoords[0] << 0.0, 0.0;
    TexCoords[1] << 0.0, 0.0;
    TexCoords[2] << 0.0, 0.0;
}

void Triangle::setVertex(int ind, Eigen::Vector4f ver){
    Vertex[ind] = ver;
}
void Triangle::setNormal(int ind, Eigen::Vector3f n){
    Normal[ind] = n;
}
void Triangle::setColor(int ind, float r, float g, float b) {
    Color[ind] = Eigen::Vector3f(r,g,b);
    return;
}
void Triangle::setTexCoord(int ind, Eigen::Vector2f uv) {
    TexCoords[ind] = uv;
}
