#ifndef MODEL_H
#define MODEL_H
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model();
    Model(QString s, bool flipUVs = 0,bool mirrorTexture = 0);
    void loadModel(QString p);
    void addFragShader(QOpenGLShader *frag);
    void addVertShader(QOpenGLShader *vert);
    void setMVPMatrix(QMatrix4x4 m,QMatrix4x4 v,QMatrix4x4 p);
    void setCameraPosValue(QVector3D &pos);
    void setModelLight(int m);
    void setLightPosValue(const QVector3D &pos);
    void processNode(aiNode *node, const aiScene *scene);
    std::vector<QOpenGLTexture *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void Draw();
    void init();
    void setTextureProprietes(bool flipUVs,bool mirrorTexture);
    //QMatrix4x4 model;
//    void setVertexAttribute(QString s);
//    void setTexCoordsAttribute(QString s);
//    void setNormalAttribute(QString s);
//    void setLightPosUniform(QString s);
//    void setCameraPosUniform(QString s);
//    void setCameraPosUniformValue(const QVector3D &v);
//    void drawNormals();
//    void setMVPUniform(QString m, QString v, QString p);
//    void programLink();
//    void setMVPUniformValue(const QMatrix4x4 &m, const QMatrix4x4 &v, const QMatrix4x4 &p);
//    void setLightPosUniformValue(const QVector3D &p);
//    void addUniform(QString s);
//    void addShader(QOpenGLShader *sh);
//    void addVertex(QVector3D v);
//    void addNormal(QVector3D n);
//    void addUV(QVector2D uv);
//    void loadTexture(QString path);
//    void addFace(QVector3D p1, QVector3D p2, QVector3D p3);
//    void addFace(QVector2D p1, QVector2D p2, QVector2D p3);
//    void create();
//    void Draw();
//    void PintFaces();
//    uint VertexCount();
//    uint NormalCount();
private:
    std::vector<Mesh> meshes;
    QOpenGLShader *frag = 0;
    QOpenGLShader *vert = 0;
    QMatrix4x4 m, v, p;
    QVector3D camPos, lightPos;
    QString directory;
    bool flipUVs = 0;
    bool mirrorTexture = 0;
    int mod;
//    QVector<QVector3D> vertices, normals, nLines;
//    QVector<QVector2D> tex_coords;
//    QVector<GLuint> vertexIndex;
//    QVector<QVector3D> textureIndex;
//    QVector<QVector3D> normalIndex;
//    QOpenGLTexture *texture;

//    QOpenGLShaderProgram program;
//    QOpenGLVertexArrayObject *vao;
//    QOpenGLBuffer *vbo, *ebo, *nbo, *nLinesBuff, *tbo;

//    GLuint vertexAttr, normalAttr, model, view, projection, lPos, cameraPos, in_tex_coords;
};

#endif // MODEL_H
