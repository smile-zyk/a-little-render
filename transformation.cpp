#include "transformation.h"
#include <cmath>
#define PI 3.1415926
Transformation::Transformation()
{
    GazeDirection<<0,0,-1;
    UpDirection<<0,1,0;
    EyePos<<0,0,10;
    ViewAngle=45.0f;
    AspectRatio=1.0f;
    ZNear=-0.1f;
    ZFar=-50.0f;
    for(int i=0;i<3;i++)
    {
        ModelPara[i]=25;
    }
    for(int i=3;i<9;i++)
    {
        ModelPara[i]=0;
    }
    ModelPara[5]=3;
    ModelPara[4]=-3;
    ModelPara[3]=1;
    ModelPara[7]=90;
    Set_ModelTransformation();
    Set_ViewportTransformation();
    Set_ProjectionTransformation();
}

void Transformation::Set_ScaleTransformation(float x,float y,float z)//放缩变换
{

    Scale<<x,0,0,0,
           0,y,0,0,
           0,0,z,0,
           0,0,0,1;

}
void Transformation::Set_TranslateTransformation(float x,float y,float z)//平移变换
{
    Translate<<1,0,0,x,
           0,1,0,y,
           0,0,1,z,
           0,0,0,1;
}
void Transformation::Set_RotationTransformation(float x,float y,float z)//旋转变换
{
    Eigen::Matrix4f transx;
    transx<<1,0,0,0,
           0,cos(x*PI/180),-sin(x*PI/180),0,
           0,sin(x*PI/180),cos(x*PI/180),0,
           0,0,0,1;
    Eigen::Matrix4f transy;
    transy<<cos(y*PI/180),0,sin(y*PI/180),0,
           0,1,0,0,
           -sin(y*PI/180),0,cos(y*PI/180),0,
           0,0,0,1;
    Eigen::Matrix4f transz;
    transz<<cos(z*PI/180),-sin(z*PI/180),0,0,
           sin(z*PI/180),cos(z*PI/180),0,0,
           0,0,1,0,
           0,0,0,1;
    Rotation=transz*transy*transx;
}
void Transformation::Set_ModelTransformation()//模型变换
{
    Set_ScaleTransformation(ModelPara[0],ModelPara[1],ModelPara[2]);
    Set_TranslateTransformation(ModelPara[3],ModelPara[4],ModelPara[5]);
    Set_RotationTransformation(ModelPara[6],ModelPara[7],ModelPara[8]);
    Model=Rotation*Translate*Scale;
}
void Transformation::Set_ViewportTransformation()//视口变换
{
    Eigen::Matrix4f transT;
    transT<<1,0,0,-EyePos[0],
           0,1,0,-EyePos[1],
           0,0,1,-EyePos[2],
           0,0,0,1;
    GazeDirection.normalize();
    UpDirection.normalize();
    Eigen::Vector3f v=GazeDirection.cross(UpDirection);
    Eigen::Matrix4f transR;
    transR<<v.x(),v.y(),v.z(),0,
           UpDirection.x(),UpDirection.y(),UpDirection.z(),0,
           -GazeDirection.x(),-GazeDirection.y(),-GazeDirection.z(),0,
           0,0,0,1;
    View=transR*transT;

}
void Transformation::Set_ProjectionTransformation()//投影变换
{
    Eigen::Matrix4f Orthographic;
    Orthographic << 1 / ((tan((ViewAngle*PI/180)/ 2)*(fabs(ZNear))*AspectRatio)), 0, 0, 0,
                     0, 1 / (tan((ViewAngle*PI/180) / 2)*(fabs(ZNear))), 0, 0,
                     0, 0, 2 / (ZNear - ZFar), 0,
                     0, 0, 0, 1;
    Projection << ZNear, 0, 0, 0,
                0, ZNear, 0, 0,
                0, 0, -ZNear + ZFar, ZNear*ZFar,
                0, 0, 1, 0;
    Projection = Orthographic * Projection;
}

Triangle Transformation::ToNDSTransformation(Triangle t)
{
    Eigen::Matrix4f mvp=Projection*View*Model;
    Eigen::Matrix4f mv_inv_trans=(View * Model).inverse().transpose();
    for(auto & v:t.Vertex)
    {
        v=mvp*v;
        v.x()/=v.w();
        v.y()/=v.w();
        v.z()/=v.w();
    }
    for(auto & n:t.Normal)
    {
        Eigen::Vector4f m(n.x(),n.y(),n.z(),0.0f);
        m=mv_inv_trans*m;
        n=m.head<3>();
        n.normalize();
    }
    return t;
}
void Transformation::Set_ModelPara(float *p)
{
    for(int i=0;i<9;i++)
    {
        ModelPara[i]=p[i];
    }
}
void Transformation::Set_ViewPara(Eigen::Vector3f gazeDirection,Eigen::Vector3f upDirection,Eigen::Vector3f eyePos,float viewAngle,float aspectRatio,float zNear,float zFar)
{
    GazeDirection=gazeDirection;
    UpDirection=upDirection;
    EyePos=eyePos;
    ViewAngle=viewAngle;
    AspectRatio=aspectRatio;
    ZNear=zNear;
    ZFar=zFar;
}
Eigen::Vector3f* Transformation::Get_ViewSpacePos(Triangle t)
{
    Eigen::Vector3f *m= new Eigen::Vector3f[3];
    int i=0;
    for(auto & v:t.Vertex)
    {
        v=Model*v;
        m[i]=v.head<3>();
        i++;
    }
    return m;
}
Eigen::Vector3f Transformation::Get_EyePos()
{
    return EyePos;
}
