#ifndef ICONTROLLER_H_INCLUDED
#define ICONTROLLER_H_INCLUDED

#include "config.h"

// Defines
#define SERVO_HOR_MAX 175 // limite superior
#define SERVO_HOR_MIN 5 // limite inferior
// 65 graus no maximo

#define SERVO_VERT_MAX 60
#define SERVO_VERT_MIN 1

class ControllerInterface {
    public:
    /************************************************************************
     controller_adjustPositions
     Ajusta as posicoes dos servos motores com base na logica de controle
     Parametros de entrada: modo de operacao, leitura atual dos sensores e ultima tecla de movimentacao pressionada
     Retorno: posicao dos servomotores
    *************************************************************************/
    virtual servosPositions adjustPositions(int operationMode, sensorsReading sensorsReading, char lastMovementKey) = 0;
};

#endif // ICONTROLLER_H_INCLUDED