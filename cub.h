#ifndef CUB_H
#define CUB_H
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
class Cub
{
public:
    Cub(QOpenGLShaderProgram &program, int vertAttr, int colorAttr, QOpenGLVertexArrayObject &vao);
    void Draw();
private:
    void initVertices();
    void initColors();
    QOpenGLShaderProgram &program;
    QOpenGLVertexArrayObject &vao;
    QOpenGLBuffer vert_vbo;
    QOpenGLBuffer color_vbo;
    int vertAttr, colorAttr;
    QVector<QVector3D> vertices,colors;
};

#endif // CUB_H
