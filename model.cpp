#include "model.h"
//QVector<QVector3D> Model::vertices, Model::normals;

Model::Model()
{

}

Model::Model(QString s, bool flipUVs, bool mirrorTexture):
flipUVs(flipUVs),mirrorTexture(mirrorTexture)
{
    loadModel(s);
}

void Model::loadModel(QString p)
{
    qDebug()<<"Load Model: "<<p;
    using namespace Assimp;
    Importer imp;
    const aiScene *scene;
    if(flipUVs)
    scene = imp.ReadFile(p.toStdString(),aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_GenNormals);
    else
        scene = imp.ReadFile(p.toStdString(),aiProcess_Triangulate|aiProcess_GenNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            qWarning() << "ERROR::ASSIMP::" << imp.GetErrorString();
            return;
        }
    QStringList lst = p.split("/");
    directory = "";
    for(int i=0;i<lst.size()-1;++i)
        directory+=lst[i]+"/";
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<QOpenGLTexture*> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        vertex.Position.setX(mesh->mVertices[i].x);
        vertex.Position.setY(mesh->mVertices[i].y);
        vertex.Position.setZ(mesh->mVertices[i].z);

        vertex.Normal.setX(mesh->mNormals[i].x);
        vertex.Normal.setY(mesh->mNormals[i].y);
        vertex.Normal.setZ(mesh->mNormals[i].z);

        if(mesh->mTextureCoords[0]){
        vertex.TexCoords.setX(mesh->mTextureCoords[0][i].x);
        vertex.TexCoords.setY(mesh->mTextureCoords[0][i].y);
        }
        else
            vertex.TexCoords = QVector2D(0,0);
        vertices.push_back(vertex);
    }
    // process indices

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
//    if(mesh->mMaterialIndex >= 0)
//    {
//        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
//                                            aiTextureType_DIFFUSE, "texture_diffuse");
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//        std::vector<Texture> specularMaps = loadMaterialTextures(material,
//                                            aiTextureType_SPECULAR, "texture_specular");
//        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//    }
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<QOpenGLTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
  ///  std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
  ///  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
   /// std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
   /// textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
   /// std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
   /// textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<QOpenGLTexture*> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<QOpenGLTexture*> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {

            aiString str;
            mat->GetTexture(type, i, &str);
            //qDebug()<<"Add TExture"<<directory+str.C_Str();
            QOpenGLTexture* texture;
            QImage *img = new QImage(directory+str.C_Str());
            if(img->isNull()){
                qDebug()<<"Error Texture"<<directory+str.C_Str();
                break;
            }
            if(mirrorTexture)
            img->mirrored();
            texture = new QOpenGLTexture(*img);
            delete img;
            texture->setMinificationFilter(QOpenGLTexture::Nearest);
            texture->setMagnificationFilter(QOpenGLTexture::Linear);
            texture->setWrapMode(QOpenGLTexture::Repeat);
            //texture.id = TextureFromFile(str.C_Str(), directory);
          //  texture.type = typeName;
           // texture.path = str;
            textures.push_back(texture);
            //delete texture;
        }
        return textures;
}

void Model::addFragShader(QOpenGLShader *frag)
{
    this->frag = frag;
}

void Model::addVertShader(QOpenGLShader *vert)
{
    this->vert = vert;
}

void Model::setMVPMatrix(QMatrix4x4 m,QMatrix4x4 v,QMatrix4x4 p)
{
    this->m = m;
    this->v = v;
    this->p = p;
}

void Model::setCameraPosValue(QVector3D &pos)
{
    camPos = pos;
}

void Model::setModelLight(int m)
{
    mod = m;
}

void Model::setLightPosValue(const QVector3D &pos)
{
    lightPos = pos;
}




//void Model::setVertexAttribute(QString s)
//{
//    vertexAttr = program.attributeLocation(s);
//}

//void Model::setTexCoordsAttribute(QString s)
//{
//    in_tex_coords = program.attributeLocation(s);
//}

//void Model::setNormalAttribute(QString s)
//{
//    normalAttr = program.attributeLocation(s);
//}

//void Model::setLightPosUniform(QString s)
//{
//    lPos = program.uniformLocation(s);
//}

//void Model::setCameraPosUniform(QString s)
//{
//    cameraPos = program.uniformLocation(s);
//}

//void Model::setCameraPosUniformValue(const QVector3D &v)
//{
//    program.bind();
//    program.setUniformValue(cameraPos,v);
//    program.release();
//}

//void Model::drawNormals()
//{

//}

//void Model::setMVPUniform(QString m, QString v, QString p)
//{
//    model = program.uniformLocation(m);
//    view = program.uniformLocation(v);
//    projection = program.uniformLocation(p);
//}

//void Model::programLink()
//{
//    if(!program.link())
//        qWarning()<<"Error link"<<program.log();
//}

//void Model::setMVPUniformValue(const QMatrix4x4 &m, const QMatrix4x4 &v, const QMatrix4x4 &p)
//{
//    program.bind();
//    program.setUniformValue(model,m);
//    program.setUniformValue(view,v);
//    program.setUniformValue(projection,p);
//    program.release();
//}

//void Model::setLightPosUniformValue(const QVector3D &p)
//{
//    program.bind();
//    program.setUniformValue(lPos,p);
//    program.release();
//}

//void Model::setUniformValue(QString s,const QMatrix4x4 &m)
//{
//    std::map<QString, GLuint>::iterator it;
//    it = uniform.find(s);
//    //qDebug()<<it->first;
//    if(it != uniform.end()){
//        program.bind();
//        program.setUniformValue(s.data()->toLatin1(),m);
//        program.release();
//    }
//    else
//        qWarning()<<"Error set uniform value, "<<s;
//}

//void Model::addAttribute(QString s)
//{
//    attribute.insert(std::pair<QString,GLuint>(s,program.attributeLocation(s)));

//}

//void Model::addUniform(QString s)
//{
//    uniform.insert(std::pair<QString,GLuint>(s,program.uniformLocation(s)));
//}

//void Model::addShader(QOpenGLShader *sh)
//{
//    if(!program.addShader(sh))
//        qWarning()<<"Error adding shader";
//}

//void Model::addVertex(QVector3D v)
//{
//    vertices.push_back(v);

//}

//void Model::addNormal(QVector3D n)
//{
//    normals.push_back(n);
//}

//void Model::addUV(QVector2D uv)
//{
//    tex_coords.push_back(uv);
//}

//void Model::loadTexture(QString path)
//{
//    texture = new QOpenGLTexture(QImage(path).mirrored());
//    texture->setMinificationFilter(QOpenGLTexture::Nearest);
//    texture->setMagnificationFilter(QOpenGLTexture::Linear);
//    texture->setWrapMode(QOpenGLTexture::Repeat);
//    program.bind();
//    program.setUniformValue("texture",0);
//}

//void Model::addFace(QVector3D p1, QVector3D p2, QVector3D p3)
//{
//    //faces.push_back(Face(p1,p2,p3));
//}

//void Model::addFace(QVector2D p1, QVector2D p2, QVector2D p3)
//{

//    vertexIndex.push_back(p1.x()-1);
//    vertexIndex.push_back(p2.x()-1);
//    vertexIndex.push_back(p3.x()-1);
//}



//void Model::create()
//{
//   // for(int i = 0 ; i<vertexIndex.size();++i)
//  //  {
//        //nLines.push_back(vertices[vertexIndex[i]]+normals[vertexIndex[i]]);
//        /*qDebug()<<vertexIndex[i]<<" "<<vertexIndex[i+1]<<" "<<vertexIndex[i+2]
//               <<vertices[vertexIndex[i]]<<" "<<vertices[vertexIndex[i+1]]<<" "<<vertices[vertexIndex[i+2]]*/;
//   // }
//    //qDebug()<<vertices.size()<<vertexIndex.size();
//    //qDebug()<<attribute.size()<<uniform.size();
//    vao = new QOpenGLVertexArrayObject();
//    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//    //ebo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

//    vao->create();
//    vao->bind();

//    vbo->create();
//    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
//    vbo->bind();
//    vbo->allocate(vertices.data(),vertices.size()*sizeof(QVector3D));
//    program.enableAttributeArray(vertexAttr);
//    program.setAttributeBuffer(vertexAttr,GL_FLOAT,0,3);
//    //program.disableAttributeArray(vertexAttr);
//    vbo->release();

//    if(normalAttr){
//        nbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//        nbo->create();
//        nbo->bind();
//        nbo->allocate(normals.data(),normals.size()*sizeof(QVector3D));
//        program.enableAttributeArray(normalAttr);
//        program.setAttributeBuffer(normalAttr,GL_FLOAT,0,3);
//        nbo->release();
//    }

//    if(in_tex_coords){
//    tbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//    tbo->create();
//    tbo->bind();
//    tbo->allocate(tex_coords.data(),tex_coords.size()*sizeof(QVector2D));
//    program.enableAttributeArray(in_tex_coords);
//    program.setAttributeArray(in_tex_coords,GL_FLOAT,0,2);
//    tbo->release();
//    }
//    //ebo->create();
//    //ebo->setUsagePattern(QOpenGLBuffer::StaticDraw);
//    //ebo->bind();
//    //ebo->allocate(vertexIndex.data(),vertexIndex.size()*sizeof(GLuint));
//    //ebo->release();

//    vao->release();
//}

void Model::Draw()
{
//    if(!program.bind()){
//        qWarning()<<program.log();
//        return;
//    }
//    vao->bind();
//    vbo->bind();
//    if(in_tex_coords){
//    tbo->bind();
//    texture->bind();
//    }
//   // ebo->bind();
//    if(normalAttr)
//        nbo->bind();
//    //glDrawElements(GL_TRIANGLES,vertexIndex.size(),GL_UNSIGNED_INT,0);
//    glDrawArrays(GL_TRIANGLES,0,vertices.size());
//    if(normalAttr)
//    nbo->release();
//    if(in_tex_coords){
//    tbo->release();
//    texture->release();
//    }
//    //ebo->release();
//    vao->release();
//    vbo->release();
//    program.release();
    for(uint i = 0;i<meshes.size();++i){

        meshes[i].setModelMatrix(m);
        meshes[i].setViewMatrix(v);
        meshes[i].setProjectonMatrix(p);
        meshes[i].setCameraPosUniformValue(camPos);
        meshes[i].setLightPosUniformValue(lightPos);
        meshes[i].setLightModel(mod);
        meshes[i].Draw();
    }

}

void Model::init()
{
    for(uint i = 0;i<meshes.size();++i){
        meshes[i].setFragmentShader(frag);
        meshes[i].setVertexShader(vert);
        meshes[i].initializeBuffers();
    }
}

void Model::setTextureProprietes(bool flipUVs, bool mirrorTexture)
{
    this->flipUVs = flipUVs;
    this->mirrorTexture = mirrorTexture;
}



//void Model::PintFaces()
//{
//    qDebug()<<normals.size()<<vertices.size();
//    for(int i = 0 ; i<vertexIndex.size();i+=3)
//    {
//        qDebug()<<vertexIndex[i]<<" "<<vertexIndex[i+1]<<" "<<vertexIndex[i+2]
//               <<vertices[vertexIndex[i]]<<" "<<vertices[vertexIndex[i+1]]<<" "<<vertices[vertexIndex[i+2]];
//    }
//}


//uint Model::VertexCount()
//{
//    return vertices.size();
//}

//uint Model::NormalCount()
//{
//    return normals.size();
//}



