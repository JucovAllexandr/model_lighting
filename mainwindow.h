#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <settings.h>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Settings settings;
    Ui::MainWindow *ui;
    QString loadPath;
    QTimer fpsTimer;
    bool flipUVs = 1;
    bool mirrorTexture = 1;
private slots:
    void loadModel();
    void openObjFile();
    void triggerUV(bool b);
    void MirrorTex(bool b);
    void translateX(int n);
    void translateY(int n);
    void translateZ(int n);
    void scaleF(double n);
    void setSpeed(double n);
    void updateFps();
    void lightX(int n);
    void lightY(int n);
    void lightZ(int n);
    void LightRotate(bool b);
    void setPhong();
    void setLambert();
    void setCelshaded();
};

#endif // MAINWINDOW_H
