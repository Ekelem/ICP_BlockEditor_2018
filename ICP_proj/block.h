#ifndef BLOCK_H
#define BLOCK_H

#include <QVector>
#include "typemanager.h"

class Block
{
public:
    Block();
private:
    QVector<DefinedType*> in_ports;
    QVector<DefinedType*> out_ports;
};

#endif // BLOCK_H
