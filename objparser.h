#ifndef OBJPARSER_H
#define OBJPARSER_H
#include <fstream>
#include <QString>
#include <QVector>
#include <model.h>

class ObjParser
{
public:
    ObjParser(QString s);
    QVector<Model*> models;
private:
    std::ifstream file;

};

#endif // OBJPARSER_H
