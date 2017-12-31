#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fpsTimer.setInterval(1);
    connect(ui->actionLoad_model,SIGNAL(triggered(bool)),this,SLOT(openObjFile()));
    connect(ui->actionFlip_UVs,SIGNAL(toggled(bool)),this,SLOT(triggerUV(bool)));
    connect(ui->actionMirror,SIGNAL(toggled(bool)),this,SLOT(MirrorTex(bool)));
    connect(ui->actionReload,SIGNAL(triggered(bool)),this,SLOT(loadModel()));
    connect(ui->spinBox_Tr_X,SIGNAL(valueChanged(int)),this,SLOT(translateX(int)));
    connect(ui->spinBox_Tr_Y,SIGNAL(valueChanged(int)),this,SLOT(translateY(int)));

    connect(ui->spinBox_Tr_Z,SIGNAL(valueChanged(int)),this,SLOT(translateZ(int)));

    connect(ui->doubleSpinBox_Sc_F,SIGNAL(valueChanged(double)),this,SLOT(scaleF(double)));

    connect(&fpsTimer,SIGNAL(timeout()),this,SLOT(updateFps()));
    connect(ui->spinBox_L_X,SIGNAL(valueChanged(int)),this,SLOT(lightX(int)));
    connect(ui->spinBox_L_Y,SIGNAL(valueChanged(int)),this,SLOT(lightY(int)));
    connect(ui->spinBox_L_Z,SIGNAL(valueChanged(int)),this,SLOT(lightZ(int)));
    settings.factor = ui->doubleSpinBox_Sc_F->value();
    settings.light.setX(ui->spinBox_L_X->value());
    settings.light.setY(ui->spinBox_L_Y->value());
    settings.light.setZ(ui->spinBox_L_Z->value());
    settings.speed = ui->doubleSpinBox_speed->value();
    connect(ui->actionRotate,SIGNAL(triggered(bool)),this,SLOT(LightRotate(bool)));
    connect(ui->doubleSpinBox_speed,SIGNAL(valueChanged(double)),this,SLOT(setSpeed(double)));
    settings.mod = 1;
    connect(ui->actionPhong,SIGNAL(triggered(bool)),this,SLOT(setPhong()));
    connect(ui->actionLambert,SIGNAL(triggered(bool)),this,SLOT(setLambert()));
    connect(ui->actionCel_shaded,SIGNAL(triggered(bool)),this,SLOT(setCelshaded()));
    ui->openGLWidget->setSettings(settings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadModel()
{
    fpsTimer.stop();
    delete ui->openGLWidget;
    ui->openGLWidget = new Widget(this,loadPath,flipUVs,mirrorTexture);
    ui->openGLWidget->setFocus();
    ui->horizontalLayout->addWidget(ui->openGLWidget);
    ui->openGLWidget->setSettings(settings);
    fpsTimer.start();
}

void MainWindow::openObjFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Obj (*.obj)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setModal(true);
    ui->openGLWidget->pause();
    if (dialog.exec()){
        // ui->openGLWidget->;
        loadPath = dialog.selectedFiles()[0];
        loadModel();
    }
    ui->openGLWidget->start();
}

void MainWindow::triggerUV(bool b)
{
    flipUVs = b;
}

void MainWindow::MirrorTex(bool b)
{
    mirrorTexture = b;
}

void MainWindow::translateX(int n)
{
    settings.translate.setX(n);
    ui->openGLWidget->setSettings(settings);
}

void MainWindow::translateY(int n)
{
    settings.translate.setY(n);
    ui->openGLWidget->setSettings(settings);
}

void MainWindow::translateZ(int n)
{
    settings.translate.setZ(n);
    ui->openGLWidget->setSettings(settings);
}

void MainWindow::scaleF(double n)
{
    settings.factor = n;
    ui->openGLWidget->setSettings(settings);
}

void MainWindow::setSpeed(double n)
{
    settings.speed = n;
    ui->openGLWidget->setSettings(settings);
}


void MainWindow::updateFps()
{
    ui->statusbar->showMessage("FPS: "+QString::number(ui->openGLWidget->getFps()));
}

void MainWindow::lightX(int n)
{
    settings.light.setX(n);
    ui->openGLWidget->setSettings(settings);
}

void MainWindow::lightY(int n)
{
    settings.light.setY(n);
    ui->openGLWidget->setSettings(settings);
}

void MainWindow::lightZ(int n)
{
    settings.light.setZ(n);
    ui->openGLWidget->setSettings(settings);
}

void MainWindow::LightRotate(bool b)
{
    if(b)
        ui->openGLWidget->startRotate();
    else
        ui->openGLWidget->stopRotate();
}

void MainWindow::setPhong()
{
    settings.mod = 1;
    ui->openGLWidget->setSettings(settings);
    ui->actionLambert->setCheckable(false);
    ui->actionCel_shaded->setCheckable(false);
}

void MainWindow::setLambert()
{
    settings.mod = 2;
    ui->openGLWidget->setSettings(settings);
    ui->actionPhong->setCheckable(false);
    ui->actionCel_shaded->setCheckable(false);
}

void MainWindow::setCelshaded()
{
    settings.mod = 3;
    ui->openGLWidget->setSettings(settings);
    ui->actionPhong->setCheckable(false);
    ui->actionLambert->setCheckable(false);
}
