#ifndef IPOSITIONER_H_INCLUDED
#define IPOSITIONER_H_INCLUDED

#include "config.h"

class PositionerInterface {
    public:
    /************************************************************************
     moveMotors
     Movimenta os motores
     Parametros de entrada: valores correspondentes aos servos
     Retorno: nenhum
    *************************************************************************/
    virtual void moveMotors(servosPositions servosPositions) = 0;
};

#endif // IPOSITIONER_H_INCLUDED