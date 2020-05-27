#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <Eigen/Eigen>
#include <Eigen/Dense>

class Triangle
{
public:
    Eigen::Vector4f Vertex[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
        /*Per vertex values*/
    Eigen::Vector3f Color[3]; //color at each vertex;
    Eigen::Vector2f TexCoords[3]; //texture u,v
    Eigen::Vector3f Normal[3]; //normal vector for each vertex
    Triangle();
    void setVertex(int ind, Eigen::Vector4f ver);
    void setNormal(int ind, Eigen::Vector3f n);
    void setColor(int ind, float r, float g, float b);
    void setTexCoord(int ind,Eigen::Vector2f uv ); /*set i-th vertex texture coordinate*/
};

#endif // TRIANGLE_H
