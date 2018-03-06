#ifndef TYPEMANAGER_H
#define TYPEMANAGER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>
#include <QFile>

#include <iostream>

typedef struct TypeInfo
{
    QString base_name;
    QString derived_from;
    quint16 width;
    bool sig;
} TypeInfo_T;

typedef struct DefinedType
{
    DefinedType() {name="undef", child.append("double");}
    DefinedType(QString new_name) {name=new_name, child.append("double");}
    DefinedType(QString new_name, QString from) {name=new_name, child.append(from);}
    DefinedType(QString new_name, QVector<QString> from) {name=new_name, child=from;}
    QString name;
    QVector<QString> child;     //according to task always 'double'
}DefinedType_T;

class TypeManager
{
public:
    TypeManager();
    bool CreateType(QString TypeName);

private:
    QMap<QString, DefinedType_T> StructTypeMap;
    QMap<QString, quint16> AtomicTypeMap;
    TypeInfo_T Info[128];

};

#endif // TYPEMANAGER_H
