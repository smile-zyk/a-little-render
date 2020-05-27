#ifndef RASTERIZER_H
#define RASTERIZER_H
#include <QImage>
#include "shader.h"
#include "transformation.h"
#include "triangle.h"
#include "OBJ_Loader.h"
#include <string>
#include <vector>
class Rasterizer
{
    Transformation trans;
    Shader shader;
    std::vector<Triangle> triangleList;
    std::string ModelPath;
    int width;
    int height;
    std::vector<float> depth_buffer;
public:
    QImage img;
    Rasterizer(int w,int h);
    void SetModelPath(std::string path);
    void Render();
    void SetTransformatin();
    void SetShader();
};

#endif // RASTERIZER_H
