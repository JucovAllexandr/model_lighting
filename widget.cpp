#include "widget.h"

Widget::Widget()
{
    initWidget();
}

Widget::Widget(QWidget *w):
    QOpenGLWidget(w)
{
    initWidget();
}

Widget::Widget(QWidget *w, QString s,bool flipUVs, bool mirrorTexture ):
    QOpenGLWidget(w),flipUVs(flipUVs), mirrorTexture(mirrorTexture)
{
    initWidget();
    path = s;
}


void Widget::pause()
{
    timer.stop();
}

void Widget::setSettings(Settings s)
{
    settings = s;
    updateSettings();

}

void Widget::start()
{
    timer.start();
}

void Widget::stopRotate()
{
    lightTimer.stop();
}

void Widget::startRotate()
{
    lightTimer.start();
}

float Widget::getFps()
{
    return fps;
}

Widget::~Widget()
{
    delete m1;
    delete lightCub;
    delete vertShader;
    delete fragShader;
    delete PhongVertShader;
    delete PhongFragShader;

}

void Widget::updateSettings()
{
    ModelM.setToIdentity();
    //qDebug()<<s.factor;
    ModelM.scale(settings.factor);
    ModelM.translate(settings.translate);
    lightPos = settings.light;
    cameraSpeed = settings.speed;
}


void Widget::initWidget()
{
    key_a = key_d = key_s = key_w = false;
    m1 = NULL;
    path = "";
    QSurfaceFormat fmt;
    fmt.setVersion( 3, 3 );
    fmt.setProfile( QSurfaceFormat::CoreProfile );
    setFormat( fmt );
    QSurfaceFormat::setDefaultFormat( fmt );
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    timer.start(1);
    frameCount = 0;

}

void Widget::initializeGL()
{
    updateSettings();
    m_logger = new QOpenGLDebugLogger( this );

       connect( m_logger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
                this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ),
                Qt::DirectConnection );

       if ( m_logger->initialize() ) {
           m_logger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
           m_logger->enableMessages();
       }

    glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
    glClearColor(0.2,0.2,0.2,1.0);
//ObjParser("../cub/untitled.obj").models[0];
    //vao.create();
    // vao.bind();
    ////m1 = ObjParser("../cub/bb8_2.obj").models[0];
    //qDebug()<<"addres"<<m1;
    ////lightCub = ObjParser("../cub/cub.obj").models[0];
    //qDebug()<<"addres"<<lightCub;
    //m1->PintFaces();
    //lightCub->PintFaces();
    //m1 = new Model();
    lightCub = new Model("../cub/cub.obj");
    vertShader = new QOpenGLShader(QOpenGLShader::Vertex);
    fragShader = new QOpenGLShader(QOpenGLShader::Fragment);


    if(!vertShader->compileSourceFile(":/new/shaders/vert.vert"))
        qWarning() << "Error compile vertex shader "<<vertShader->log();
    if(!fragShader->compileSourceFile(":/new/shaders/frag.frag"))
        qWarning() << "Error compile feagment shader "<<fragShader->log();

    m1 = new Model(path,flipUVs,mirrorTexture);
    PhongVertShader = new QOpenGLShader(QOpenGLShader::Vertex,this->context());
    PhongFragShader = new QOpenGLShader(QOpenGLShader::Fragment,this->context());

    if(!PhongVertShader->compileSourceFile(":/new/shaders/phongvert.vert"))
        qWarning() << "Error compile vertex shader "<<vertShader->log();
    if(!PhongFragShader->compileSourceFile(":/new/shaders/phongfrag.frag"))
        qWarning() << "Error compile feagment shader "<<fragShader->log();

    m1->addVertShader(PhongVertShader);
    m1->addFragShader(PhongFragShader);
    m1->init();

    lightCub->addVertShader(vertShader);
    lightCub->addFragShader(fragShader);
    lightCub->init();
   // m1->programLink();
   // lightCub->programLink();

  //  m1->setVertexAttribute("vertex");
  //  m1->setNormalAttribute("normal");
  //  m1->setMVPUniform("M","V","P");
  //  m1->setLightPosUniform("lPos");
  //  lightCub->setMVPUniform("M","V","P");
   // m1->setCameraPosUniform("camPos");
  //  m1->setTexCoordsAttribute("in_tex_coords");
    //m1->addUniform("MVP");
   // m1->loadTexture("../cub/txture.png");
  //  lightCub->setVertexAttribute("vertexAttr");
   // lightCub->addUniform("MVP");
    //vertAttr = program.attributeLocation("vertexAttr");
    //colorAttr = program.attributeLocation("colorAttr");
    //mvpAttr = program.uniformLocation("MVP");

   // m1->create();
  //  lightCub->create();
    // c = new Cub(program,vertAttr,colorAttr,vao);
    // vao.release();

    //distance = 6;
    //View.setToIdentity();
    camPos = QVector3D(0,0,10);
    camFront = QVector3D(0,0,-1);
    camUp = QVector3D(0,1,0);
    camWUp = camUp;

   // View.lookAt(camPos,camFront,camUp);
    ModelM.setToIdentity();
    //ModelM.scale(0.1);
    //ModelM.translate(0,0,0);

    //    for(int i=0;i<10;i++){
    //    Model[i].setToIdentity();
    //    Model[i].translate(i+1,0,-i);
    //}
    time.start();

    //lightPos = QVector3D(0,100,100);


    connect(&lightTimer,SIGNAL(timeout()),this,SLOT(updateLightPos()));
    lightTimer.start(1);
    //Model2.setToIdentity();
    //Model2.translate(4,0,0);
}

void Widget::paintGL()
{
    //qDebug()<<settings.translate;


    QVector3D tmp;
    if (key_w)
       camPos += cameraSpeed * camFront;
    if (key_s)
       camPos -= cameraSpeed * camFront;
       //qDebug()<<"S";

    if (key_a){
        tmp = QVector3D::crossProduct(camFront,camUp);
        tmp.normalize();
        camPos -=  tmp* cameraSpeed;}
        //qDebug()<<"A";


    if (key_d){
        tmp = QVector3D::crossProduct(camFront,camUp);
        tmp.normalize();
        camPos += tmp * cameraSpeed;
        //qDebug()<<"D";
    }

    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // vao.bind();


    QMatrix4x4 View;
    View.lookAt(camPos,camPos+camFront,camUp);
    //m1->setMVPUniformValue(ModelM, View, Projection);
    //m1->setLightPosUniformValue(rotLight*lightPos);
    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(rotLight*lightPos);
    //m1->setCameraPosUniformValue(camPos);
    //lightCub->setMVPUniformValue(model, View,Projection);
    lightCub->setMVPMatrix(model,View,Projection);
    lightCub->Draw();

    m1->setCameraPosValue(camPos);
    m1->setLightPosValue(rotLight*lightPos);
    m1->setMVPMatrix(ModelM,View,Projection);
    m1->setModelLight(settings.mod);
    m1->Draw();
    //QMatrix4x4 mMatrix;
    //QMatrix4x4 vMatrix;
    //Model.rotate(0.5,1,1,0);
    //Model2.rotate(0.9,1,1,0);
    //    for(int i=0;i<10;i++){
    //    Model[i].rotate(1+i/2,1,1,0);
    //
    //    c->Draw();}

    ++frameCount;
    if(time.elapsed() >= 1000){
        //qDebug()<<frameCount/(time.elapsed() /1000.0f);
        fps = frameCount/(time.elapsed() /1000.0f);
        //qDebug()<<camPos+camFront;
        frameCount = 0;
        time.restart();
    }




    //    program.setUniformValue(mvpAttr,Projection*View*Model2);
    //    c->Draw();
    //program.release();
    //    vao.release();


}

void Widget::resizeGL(int w, int h)
{
    glViewport( 0, 0, w, h );
    Projection.setToIdentity();
    Projection.perspective(45,(float)w/h,0.1,10000);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
    lastPos = event->pos();
    pressed = 1;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(pressed){

        int dx = event->pos().x() - lastPos.x();
        int dy = event->pos().y() - lastPos.y();
        lastPos = event->pos();
        // View.setToIdentity();
//        alpha -= dx;
//        while (alpha < 0) {
//            alpha += 360;
//        }
//        while (alpha >= 360) {
//            alpha -= 360;
//        }
//        QMatrix4x4 m;
//        m.rotate(alpha,QVector3D(0,1,0));
//        centerEye = m*QVector3D(0,0,distance);
        float s = 0.1;
         yaw += dx*s;
         pitch += dy*s;

//        if(yaw > 360)
//          yaw =  0;
//        if(yaw < -360)
//          yaw = 0;

        if(pitch > 89.0f)
          pitch =  89.0f;
        if(pitch < -89.0f)
          pitch = -89.0f;
        QVector3D f;
        //qDebug()<<yaw;
        f.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
        f.setY(sin(qDegreesToRadians(pitch)));
        f.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
        f.normalize();
        //View.setToIdentity();
       // centerEye += QVector3D(xcam,dy,ycam);
       //qDebug()<< f;
      camFront =  f;
      //QVector3D right;
      //right.crossProduct(camFront,camWUp);
     // right.normalize();
      //camUp.crossProduct(right,camFront);
      //camUp.normalize();

     //  Up    = glm::normalize(glm::cross(Right, Front));
     //   View.lookAt(camPos,camFront,camUp);
       // View.lookAt(QVector3D(dx,0,6),QVector3D(0,0,0),QVector3D(0,1,0));

        //Model.rotate(dy*0.01,1,0,0);
        //Model.rotate(dx*0.01,0,1,0);
        //Model.rotate(1,0,0,1);
        //Model.rotate(1,0,1,0);
        //Model.rotate(1,1,0,0);
        //qWarning()<<dx<<" "<<dy<<endl;
        //qWarning()<<dx*0.1<<" "<<dy*0.1<<endl;
       //lastPos= event->pos();
    }

}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = 0;
}

void Widget::wheelEvent(QWheelEvent *event)
{
//    int delta = event->delta();
//    if (delta < 0)
//        distance *= 1.1;
//    else if (delta > 0)
//        distance *= 0.9;
//    centerEye.setZ(distance);
//    View.lookAt(centerEye,QVector3D(0,0,0),QVector3D(0,1,0));
//draw = true;
}

void Widget::keyPressEvent(QKeyEvent *e)
{


    if (e->key() == Qt::Key_W)
       key_w = true;
    if (e->key() == Qt::Key_S)
       key_s = true;
    if (e->key() == Qt::Key_A)
       key_a = true;
    if (e->key() == Qt::Key_D)
       key_d = true;



    //View.setToIdentity();
    // View.lookAt(camPos,camPos+camFront,camUp);
}

void Widget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_W)
       key_w = false;
    if (e->key() == Qt::Key_S)
       key_s = false;
    if (e->key() == Qt::Key_A)
       key_a = false;
    if (e->key() == Qt::Key_D)
        key_d = false;
}

void Widget::updateLightPos()
{
   ++ang;
   if(ang>360) ang = 0;
   rotLight.setToIdentity();
   rotLight.rotate(ang,0,1,0);
   //lightPos.setX(lightPos.x()+1);
   //rotLight.translate(lightPos.x(),lightPos.y(),lightPos.z());
   //lightPos = m*lightPos;
}


