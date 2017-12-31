#include "mesh.h"


Mesh::Mesh(std::vector<Vertex > vertices, std::vector<unsigned int> indeces, std::vector<QOpenGLTexture *> textures)
{

    this->vertices = vertices;
    this->indeces = indeces;
    this->textures = textures;
    //initializeBuffers();
}

void Mesh::Draw()
{
    program->bind();
    vao->bind();
    ebo->bind();


   // txt->bind();
    program->setUniformValue("texture",0);
    for(uint i=0;i<textures.size();++i){
       // qDebug()<<"Texture size"<<textures.size();
        textures[i]->bind();
    }
    program->setUniformValue("hasTexture",(bool)textures.size());
    glDrawElements(GL_TRIANGLES,indeces.size(),GL_UNSIGNED_INT,0);
    //glDrawArrays(GL_TRIANGLES,0,vertices.size());
    for(uint i=0;i<textures.size();++i){
        textures[i]->release();
    }
   // txt->release();
    vao->release();
    ebo->release();
    program->release();

}

void Mesh::setCameraPosUniformValue(QVector3D pos)
{
    program->bind();
    program->setUniformValue("camPos",pos);
    program->release();
}

void Mesh::setLightPosUniformValue(QVector3D pos)
{
    program->bind();
    program->setUniformValue("lPos",pos);
    program->release();
}

void Mesh::setFragmentShader(QOpenGLShader *frag)
{
    this->frag = frag;
}

void Mesh::setVertexShader(QOpenGLShader *vert)
{
    this->vert = vert;
}

void Mesh::setLightModel(int m)
{
    program->bind();
    program->setUniformValue("lightning",m);
    program->release();
}

void Mesh::setModelMatrix(QMatrix4x4 m)
{
    program->bind();
    program->setUniformValue("M",m);
    program->release();
}

void Mesh::setViewMatrix(QMatrix4x4 v)
{
    program->bind();
    program->setUniformValue("V",v);
    program->release();
}

void Mesh::setProjectonMatrix(QMatrix4x4 p)
{
    program->bind();
    program->setUniformValue("P",p);
    program->release();
}



void Mesh::initializeBuffers()
{
    program = new QOpenGLShaderProgram();
    program->create();
    //program->bind();
    //qDebug()<<vert;
    if(!program->addShader(vert))
        qWarning()<<"Error adding vertex shader";
    if(!program->addShader(frag))
        qWarning()<<"Error adding fragment shader";

    program->link();
    vao = new QOpenGLVertexArrayObject();
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    vao->create();
    vao->bind();
//    std::vector<QVector3D> vv;
//    for(int i = 0;i<vertices.size();++i ){
//        vv.push_back(vertices[i].Position);
//    qDebug()<<i<<" "<<vertices[i].Position;
//    }
//    for(int i = 0;i<indeces.size();++i )
//    qDebug()<<i<<" "<<indeces[i];

    vbo->create();
    vbo->bind();
    vbo->allocate(vertices.data(),vertices.size()*sizeof(Vertex));
   // vbo->allocate(vv.data(),vv.size()*sizeof(QVector3D));
    program->enableAttributeArray(0);
    program->setAttributeBuffer(0,GL_FLOAT,0,3,sizeof(Vertex));
    program->enableAttributeArray(1);
    program->setAttributeBuffer(1,GL_FLOAT,offsetof(Vertex,Vertex::Normal),3,sizeof(Vertex));
    program->enableAttributeArray(2);
    program->setAttributeBuffer(2,GL_FLOAT,offsetof(Vertex,Vertex::TexCoords),2,sizeof(Vertex));

    ebo->create();
    ebo->bind();
    ebo->allocate(indeces.data(),indeces.size()*sizeof(unsigned int));

    vao->release();
    ebo->release();
    program->release();
}

Mesh::~Mesh()
{

    for(uint i=0;i<textures.size();++i)
        if(!textures[i])
            delete textures[i];
    if(!program)
    delete program;
    delete vao;
    delete vbo;
    delete ebo;

}

