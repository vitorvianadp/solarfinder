#ifndef POSITIONER_H_INCLUDED
#define POSITIONER_H_INCLUDED

#include "config.h"
#include "iPositioner.h"
#ifndef DEBUG
    #include <ESP32Servo.h>
#endif

class PositionerClass: public PositionerInterface{
    public:
    /************************************************************************
     construtor
    *************************************************************************/
    PositionerClass();

    /************************************************************************
     moveMotors
     Movimenta os motores
     Parametros de entrada: valores correspondentes aos servos
     Retorno: nenhum
    *************************************************************************/
    void moveMotors(servosPositions servosPositions);

    private:
    /************************************************************************
     begin
     Inicia os servomotores
     Parametros de entrada: nenhum
     Retorno: nenhum
    *************************************************************************/
    void begin();

    #ifndef DEBUG
        Servo horizontal; // servo horizontal no pino 9
        Servo vertical; // servo vertical no pino 10
    #endif
};

extern PositionerClass Positioner;

#endif // POSITIONER_H_INCLUDED