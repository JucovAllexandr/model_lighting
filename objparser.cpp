#include "objparser.h"
#include <string>
//#include <iostream>
#include <QDebug>
ObjParser::ObjParser(QString s)
{
    file.open(s.toStdString().c_str());
    QVector<QVector3D> nm,vc;
    QVector<QVector2D> vt;
    Model *m;
    while(!file.eof()){
        std::string ln;
        std::getline(file,ln);
        //std::cout<<ln<<std::endl;
        QString t(ln.c_str());
        QStringList list =  t.split(' ');

        if(list[0]=="o"){
            //qDebug() <<list;
            //m = new Model(((QString)list[1]).toStdString());

            models.push_back(m);
            continue;
        }
        if(list[0]=="v"){
            //qDebug()<<list;
            //qDebug()<<QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat());
            vc.push_back(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
            continue;
        }
        if(list[0]=="vt"){
            vt.push_back(QVector2D(list[1].toFloat(), list[2].toFloat()));
            continue;
        }
        if(list[0]=="vn")
        {
            //qDebug()<<list;
            nm.push_back(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
            continue;
        }
        if(list[0]=="f")
        {
            // m->VertexCount();
            //qDebug()<<list;
            QVector3D p1,p2,p3;
            QStringList t = list[1].split('/');
            if(t[1].isEmpty()){
            //qDebug()<<t<<t.size();
            p1 = QVector3D(t[0].toInt(),0,t[2].toInt());
            t = list[2].split('/');
            // qDebug()<<p1;
            p2 = QVector3D(t[0].toInt(),0,t[2].toInt());
            t = list[3].split('/');
            // qDebug()<<p2;
            p3 = QVector3D(t[0].toInt(),0,t[2].toInt());
            //  qDebug()<<p3;

            }
            else{

                p1 = QVector3D(t[0].toInt(),t[1].toInt(),t[2].toInt());
                t = list[2].split('/');

                p2 = QVector3D(t[0].toInt(),t[1].toInt(),t[2].toInt());
                t = list[3].split('/');

                p3 = QVector3D(t[0].toInt(),t[1].toInt(),t[2].toInt());

                m->addUV(vt[p1.y()-1]);
                m->addUV(vt[p2.y()-1]);
                m->addUV(vt[p3.y()-1]);
            }

            m->addNormal(nm[p1.z()-1]);
            m->addNormal(nm[p2.z()-1]);
            m->addNormal(nm[p3.z()-1]);

            m->addVertex(vc[p1.x()-1]);
            m->addVertex(vc[p2.x()-1]);
            m->addVertex(vc[p3.x()-1]);
            m->addFace(p1,p2,p3);
            continue;
        }


    }
}

