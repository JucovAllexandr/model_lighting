#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>
struct Vertex{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
};


class Mesh
{
public:

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indeces;
    std::vector<QOpenGLTexture*> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indeces, std::vector<QOpenGLTexture*> textures);
    void Draw();
    void setCameraPosUniformValue(QVector3D pos);
    void setLightPosUniformValue(QVector3D pos);
    void setFragmentShader(QOpenGLShader *frag);
    void setVertexShader(QOpenGLShader *vert);
    void setLightModel(int m);
    void setModelMatrix(QMatrix4x4 m);
    void setViewMatrix(QMatrix4x4 v);
    void setProjectonMatrix(QMatrix4x4 p);
    void initializeBuffers();
    ~Mesh();
private:
    QOpenGLShaderProgram *program=0;
    QOpenGLVertexArrayObject *vao=0;
    QOpenGLBuffer *vbo=0,*ebo=0;
    QOpenGLShader *frag=0,*vert=0;
};

#endif // MESH_H
