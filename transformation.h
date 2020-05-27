#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <triangle.h>
#include <Eigen/Eigen>
#include <Eigen/Dense>
//三维坐标转换器
class Transformation
{
    public:
    Eigen::Vector3f GazeDirection;//相机面朝方向
    Eigen::Vector3f UpDirection;//相机上方方向
    Eigen::Vector3f EyePos;//相机位置
    float ViewAngle;//相机视角
    float AspectRatio;//相机视野长宽比
    float ZFar;
    float ZNear;
    float ModelPara[9];//模型变换参数
    Eigen::Matrix4f Scale;
    Eigen::Matrix4f Translate;
    Eigen::Matrix4f Rotation;
    Eigen::Matrix4f Model;
    Eigen::Matrix4f View;
    Eigen::Matrix4f Projection;

    Transformation();
    void Set_ScaleTransformation(float x,float y,float z);//放缩变换
    void Set_TranslateTransformation(float x,float y,float z);//平移变换
    void Set_RotationTransformation(float x,float y,float z);//旋转变换
    void Set_ModelTransformation();//模型变换
    void Set_ViewportTransformation();//视口变换
    void Set_ProjectionTransformation();//投影变换
    Triangle ToNDSTransformation(Triangle t);
    Eigen::Vector3f* Get_ViewSpacePos(Triangle t);
    Eigen::Vector3f Get_EyePos();
    void Set_ModelPara(float *p);
    void Set_ViewPara(Eigen::Vector3f gazeDirection,Eigen::Vector3f upDirection,Eigen::Vector3f eyePos,float viewAngle,float aspectRatio,float zNear,float zFar);
};

#endif // TRANSFORMATION_H
