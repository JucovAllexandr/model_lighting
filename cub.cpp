#include "cub.h"

Cub::Cub(QOpenGLShaderProgram &program, int vertAttr, int colorAttr, QOpenGLVertexArrayObject &vao):
    program(program),vertAttr(vertAttr), colorAttr(colorAttr),vao(vao)
{
    initVertices();
    initColors();
    vert_vbo.create();
    vert_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vert_vbo.bind();
    vert_vbo.allocate(vertices.data(),6*vertices.size()*sizeof(QVector3D));

    color_vbo.create();
    color_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    color_vbo.bind();
    color_vbo.allocate(colors.data(),6*vertices.size()*sizeof(QVector3D));
}

void Cub::Draw()
{

    //program.setAttributeArray(vertAttr,vertices.constData());
    //program.setAttributeArray(colorAttr,colors.constData());
    vert_vbo.bind();
    program.enableAttributeArray(vertAttr);
    program.setAttributeBuffer(vertAttr,GL_FLOAT,0,3);

    color_vbo.bind();
    program.enableAttributeArray(colorAttr);
    program.setAttributeBuffer(colorAttr,GL_FLOAT,0,3);

    glDrawArrays(GL_TRIANGLES,0,vertices.size());

    program.disableAttributeArray(vertAttr);
    program.disableAttributeArray(colorAttr);
}

void Cub::initVertices()
{
    vertices<<QVector3D(-1.0f,-1.0f,-1.0f)
           <<QVector3D( -1.0f,-1.0f, 1.0f)
          <<QVector3D(-1.0f, 1.0f, 1.0f)
         <<QVector3D(1.0f, 1.0f,-1.0f)
        <<QVector3D(-1.0f,-1.0f,-1.0f)
       <<QVector3D(-1.0f, 1.0f,-1.0f)
      <<QVector3D(1.0f,-1.0f, 1.0f)
     <<QVector3D(-1.0f,-1.0f,-1.0f)
    <<QVector3D(1.0f,-1.0f,-1.0f)
    <<QVector3D(1.0f, 1.0f,-1.0f)
    <<QVector3D(1.0f,-1.0f,-1.0f)
    <<QVector3D(-1.0f,-1.0f,-1.0f)
    <<QVector3D(-1.0f,-1.0f,-1.0f)
    <<QVector3D(-1.0f, 1.0f, 1.0f)
    <<QVector3D(-1.0f, 1.0f,-1.0f)
    <<QVector3D(1.0f,-1.0f, 1.0f)
    <<QVector3D(-1.0f,-1.0f, 1.0f)
    <<QVector3D(-1.0f,-1.0f,-1.0f)
    <<QVector3D(-1.0f, 1.0f, 1.0f)
    <<QVector3D(-1.0f,-1.0f, 1.0f)
    <<QVector3D(1.0f,-1.0f, 1.0f)
    <<QVector3D(1.0f, 1.0f, 1.0f)
    <<QVector3D(1.0f,-1.0f,-1.0f)
    <<QVector3D(1.0f, 1.0f,-1.0f)
    <<QVector3D(1.0f,-1.0f,-1.0f)
    <<QVector3D(1.0f, 1.0f, 1.0f)
    <<QVector3D(1.0f,-1.0f, 1.0f)
    <<QVector3D(1.0f, 1.0f, 1.0f)
    <<QVector3D(1.0f, 1.0f,-1.0f)
    <<QVector3D(-1.0f, 1.0f,-1.0f)
    <<QVector3D(1.0f, 1.0f, 1.0f)
    <<QVector3D(-1.0f, 1.0f,-1.0f)
    <<QVector3D(-1.0f, 1.0f, 1.0f)
    <<QVector3D(1.0f, 1.0f, 1.0f)
    <<QVector3D(-1.0f, 1.0f, 1.0f)
    <<QVector3D(1.0f,-1.0f, 1.0f);


}

void Cub::initColors()
{
    colors <<QVector3D( 0.583f,  0.771f,  0.014f)
          <<QVector3D(0.609f,  0.115f,  0.436f)
         <<QVector3D( 0.327f,  0.483f,  0.844f)
        <<QVector3D(0.822f,  0.569f,  0.201f)
       <<QVector3D(0.435f,  0.602f,  0.223f)
      <<QVector3D(0.310f,  0.747f,  0.185f)
     <<QVector3D(0.597f,  0.770f,  0.761f)
    <<QVector3D(0.559f,  0.436f,  0.730f)
   <<QVector3D(0.359f,  0.583f,  0.152f)
   <<QVector3D(0.483f,  0.596f,  0.789f)
   <<QVector3D(0.559f,  0.861f,  0.639f)
   <<QVector3D(0.195f,  0.548f,  0.859f)
   <<QVector3D(0.014f,  0.184f,  0.576f)
   <<QVector3D(0.771f,  0.328f,  0.970f)
   <<QVector3D(0.406f,  0.615f,  0.116f)
   <<QVector3D(0.676f,  0.977f,  0.133f)
   <<QVector3D(0.971f,  0.572f,  0.833f)
   <<QVector3D(0.140f,  0.616f,  0.489f)
   <<QVector3D(0.997f,  0.513f,  0.064f)
   <<QVector3D(0.945f,  0.719f,  0.592f)
   <<QVector3D(0.543f,  0.021f,  0.978f)
   <<QVector3D(0.279f,  0.317f,  0.505f)
   <<QVector3D(0.167f,  0.620f,  0.077f)
   <<QVector3D(0.347f,  0.857f,  0.137f)
   <<QVector3D(0.055f,  0.953f,  0.042f)
   <<QVector3D(0.714f,  0.505f,  0.345f)
   <<QVector3D(0.783f,  0.290f,  0.734f)
   <<QVector3D(0.722f,  0.645f,  0.174f)
   <<QVector3D(0.302f,  0.455f,  0.848f)
   <<QVector3D(0.225f,  0.587f,  0.040f)
   <<QVector3D(0.517f,  0.713f,  0.338f)
   <<QVector3D(0.053f,  0.959f,  0.120f)
   <<QVector3D(0.393f,  0.621f,  0.362f)
   <<QVector3D(0.673f,  0.211f,  0.457f)
   <<QVector3D(0.820f,  0.883f,  0.371f)
   <<QVector3D(0.982f,  0.099f,  0.879f);


}



