#include "shader.h"
#include <algorithm>
#include <cmath>
Shader::Shader()
{
    auto l1 = Light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = Light{{-20, 20, 0}, { 500, 500, 500}};
    std::vector<Light> l={l1,l2};
    Set_LightList(l);
    Set_LightPara(0.005,0.7937,150,10);
};
void Shader::Set_ShaderPara(Eigen::Vector3f v,Eigen::Vector3f c,Eigen::Vector3f n,Eigen::Vector2f t,Eigen::Vector3f e)
{
    ViewPos=v;
    Color=c;
    Normal=n;
    TexCoord=t;
    EyePos=e;
}
void Shader::Set_LightPara(float a, float s,int p,float amb)
{
    ka=a;
    ks=s;
    kp=p;
    amb_light_intensity=amb;
}
void Shader::Set_LightList(std::vector<Light> l)
{
    LightList=l;
}
Eigen::Vector3f Shader::PhongShader()
{
    Eigen::Vector3f KA={ka,ka,ka};
    Eigen::Vector3f KS={ks,ks,ks};
    Eigen::Vector3f KD={Color.x()/255.0f,Color.y()/255.0f,Color.z()/255.0f};
    Eigen::Vector3f amb_intensity={amb_light_intensity,amb_light_intensity,amb_light_intensity};
     Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : LightList)
        {
            float r_2 = pow((light.position.x() - ViewPos.x()), 2) + pow((light.position.y() - ViewPos.y()), 2) + pow((light.position.z() - ViewPos.z()), 2);
            Eigen::Vector3f intensity = light.intensity / r_2;
            Eigen::Vector3f light_vec = (light.position - ViewPos).normalized();
            Eigen::Vector3f eye_vec = (EyePos - ViewPos).normalized();
            Eigen::Vector3f ambient = KA.cwiseProduct(amb_intensity);
            Eigen::Vector3f diffuse = KD.cwiseProduct(intensity)*fmax(0, light_vec.dot(Normal));
            Eigen::Vector3f specular = KS.cwiseProduct(intensity)*pow(fmax(0, Normal.dot((light_vec + eye_vec).normalized())), kp);
            result_color += (ambient + diffuse + specular);
        }

    return result_color * 255.0f;
}
