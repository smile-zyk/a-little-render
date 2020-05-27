#ifndef SHADER_H
#define SHADER_H
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <vector>
class Light
{
public :
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
    Light(Eigen::Vector3f p,Eigen::Vector3f i):position(p),intensity(i){};
};

class Shader//片元着色器(fragment shader)
{
    Eigen::Vector3f ViewPos;//片元在三维空间中的坐标
    Eigen::Vector3f Color;//片元的颜色
    Eigen::Vector3f Normal;//片元的法线向量
    Eigen::Vector2f TexCoord;//片元的纹理坐标
    Eigen::Vector3f EyePos;//观察位置
    std::vector<Light> LightList;//光源表
    float ks;//高光系数
    float ka;//环境光系数
    float amb_light_intensity;//环境光强度
    int kp;//高光指数
public:
    Shader();
    void Set_LightPara(float s, float a,int p,float amb);
    void Set_LightList(std::vector<Light> l);
    void Set_ShaderPara(Eigen::Vector3f v,Eigen::Vector3f c,Eigen::Vector3f n,Eigen::Vector2f t,Eigen::Vector3f e);
    Eigen::Vector3f PhongShader();
};

#endif // SHADER_H
