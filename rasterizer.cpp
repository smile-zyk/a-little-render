#include "rasterizer.h"
#include <vector>
#include <QPoint>
#include <QRgb>
Rasterizer::Rasterizer(int w,int h):trans(),shader(),img(w,h,QImage::Format_RGB32)
{
    img.fill(Qt::black);
    width=w;
    height=h;
    depth_buffer.resize(w*h);
    std::fill(depth_buffer.begin(), depth_buffer.end(), std::numeric_limits<float>::infinity());
}
//计算二维坐标的重心坐标
static Eigen::Vector3f computeBarycentric2D(float x, float y, const Eigen::Vector4f* v){
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return Eigen::Vector3f(c1,c2,c3);
}

static bool insideTriangle(int x, int y, const Eigen::Vector4f* _v){
    Eigen::Vector3f v[3];
    for(int i=0;i<3;i++)
        v[i] = {_v[i].x(),_v[i].y(), 1.0};
    Eigen::Vector3f f0,f1,f2;
    f0 = v[1].cross(v[0]);
    f1 = v[2].cross(v[1]);
    f2 = v[0].cross(v[2]);
    Eigen::Vector3f p(x,y,1.);
    if((p.dot(f0)*f0.dot(v[2])>0) && (p.dot(f1)*f1.dot(v[0])>0) && (p.dot(f2)*f2.dot(v[1])>0))
        return true;
    return false;
}

template <class T1>
T1 interpolate(Eigen::Vector3f bary,float viewdepth, T1* attr,float* vd)
{
    return viewdepth*(bary.x()*attr[0]/vd[0]+bary.y()*attr[1]/vd[1]+bary.z()*attr[2]/vd[2]);
}

void Rasterizer::Render()
{
//    std::cout<<trans.Model<<std::endl;
//    std::cout<<trans.View<<std::endl;
//    std::cout<<trans.Projection<<std::endl;
    for(auto t:triangleList)
    {
        Triangle tri;
        tri=trans.ToNDSTransformation(t);

        Eigen::Vector3f* view_space_pos=trans.Get_ViewSpacePos(t);
        for(auto &v:tri.Vertex)
        {
            v.x()=0.5*width*(v.x()+1.0);
            v.y()=0.5*width*(v.y()+1.0);
        }
        int max_x, max_y, min_x, min_y;
        max_x = (int)fmax(fmax(tri.Vertex[0].x(), tri.Vertex[1].x()), tri.Vertex[2].x());
        min_x = (int)fmin(fmin(tri.Vertex[0].x(), tri.Vertex[1].x()), tri.Vertex[2].x());
        max_y = (int)fmax(fmax(tri.Vertex[0].y(), tri.Vertex[1].y()), tri.Vertex[2].y());
        min_y = (int)fmin(fmin(tri.Vertex[0].y(), tri.Vertex[1].y()), tri.Vertex[2].y());
        for(int x=min_x;x<=max_x;x++)
            for (int y = min_y; y <= max_y; y++)
            {
                if(x>0&&x<width&&y>0&&y<height)
                {
                    if (insideTriangle(x, y, tri.Vertex))
                    {
                        Eigen::Vector3f Barycentric= computeBarycentric2D(x, y, tri.Vertex);
                        float vd[3]={tri.Vertex[0].w(),tri.Vertex[1].w(),tri.Vertex[2].w()};
                        auto view_space_depth = 1.0 / (Barycentric.x() / tri.Vertex[0].w() + Barycentric.y() / tri.Vertex[1].w() + Barycentric.z() / tri.Vertex[2].w());
                        if (-view_space_depth < depth_buffer[width*y + x])
                        {
                            depth_buffer[width*y + x]=-view_space_depth;
                            Eigen::Vector3f interpolated_normal=interpolate<Eigen::Vector3f>(Barycentric,view_space_depth,tri.Normal,vd);
                            Eigen::Vector2f interpolated_texcoords=interpolate<Eigen::Vector2f>(Barycentric,view_space_depth,tri.TexCoords,vd);
                            Eigen::Vector3f interpolated_viewpos=interpolate<Eigen::Vector3f>(Barycentric,view_space_depth,view_space_pos,vd);
                            shader.Set_ShaderPara(interpolated_viewpos,Eigen::Vector3f(148,121.0,92.0),interpolated_normal,interpolated_texcoords,trans.Get_EyePos());
                            Eigen::Vector3f color= shader.PhongShader();
                            if(color.x()>255) color[0]=255.0f;
                            if(color.y()>255) color[1]=255.0f;
                            if(color.z()>255) color[2]=255.0f;
                            QRgb c=qRgb(color.x(),color.y(),color.z());
                            img.setPixel(QPoint(x,height-y),c);
                            std::cout<<x<<" "<<y<<std::endl;
                            std::cout<<color.x()<<" "<<color.y()<<" "<<color.z()<<std::endl;
                        }
                    }
                }
            }
    }
}

void Rasterizer::SetModelPath(std::string path)
{
    ModelPath=path;
    objl::Loader Loader;
    Loader.LoadFile(path);
    for(auto mesh:Loader.LoadedMeshes)
    {
       for(int i=0;i<mesh.Vertices.size();i+=3)
       {
           Triangle t;
           for(int j=0;j<3;j++)
           {
               t.setVertex(j,Eigen::Vector4f(mesh.Vertices[i+j].Position.X,mesh.Vertices[i+j].Position.Y,mesh.Vertices[i+j].Position.Z,1.0));
               t.setNormal(j,Eigen::Vector3f(mesh.Vertices[i+j].Normal.X,mesh.Vertices[i+j].Normal.Y,mesh.Vertices[i+j].Normal.Z));
               t.setTexCoord(j,Eigen::Vector2f(mesh.Vertices[i+j].TextureCoordinate.X, mesh.Vertices[i+j].TextureCoordinate.Y));
           }
           triangleList.push_back(t);
       }
    }
}

void Rasterizer::SetTransformatin()
{

}
void Rasterizer::SetShader()
{

}
