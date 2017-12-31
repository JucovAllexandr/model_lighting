#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <settings.h>
//#include "cub.h"
#include <QMouseEvent>
#include <QTimer>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTime>
#include "model.h"
#include <QtMath>
#include <QOpenGLDebugLogger>
class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit Widget();
    Widget(QWidget *w);
    Widget(QWidget *w,QString s,bool flipUVs = 0, bool mirrorTexture = 0);
    void pause();
    void setSettings(Settings s);
    void start();
    void stopRotate();
    void startRotate();
    float getFps();
    ~Widget();

private:
    Settings settings;
    float fps;
    bool flipUVs = 0;
    float cameraSpeed ;
    bool mirrorTexture = 0;
    QString path;
    Model *m1,*lightCub;
    QOpenGLShader *vertShader, *fragShader;
    QOpenGLShader *PhongVertShader, *PhongFragShader;
    QOpenGLDebugLogger *m_logger;
    QMatrix4x4 Projection,ModelM;
    QMatrix4x4 rotLight;
    QVector3D camPos, camFront, camUp, lightPos,camWUp;
    int vertAttr, colorAttr, mvpAttr;
    float yaw=-90,pitch=0, ang = 0;
    int alpha=0;
    bool key_w,key_s,key_a,key_d;
    QTimer timer, lightTimer;
    QTime time;
    QPoint lastPos;
    float frameCount;
    bool pressed;
    void updateSettings();
    void initWidget();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
private slots:
    void onMessageLogged( QOpenGLDebugMessage message )
    {
        qDebug() << message;
    }
    void updateLightPos();
};

#endif // WIDGET_H
