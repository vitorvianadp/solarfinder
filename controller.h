#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include "config.h"
#include "iController.h"

class ControllerClass: public ControllerInterface{
    public:
    /************************************************************************
     construtor
    *************************************************************************/
    ControllerClass();

    /************************************************************************
     adjustPositions
     Ajusta as posicoes dos servos motores com base na logica de controle
     Parametros de entrada: modo de operacao, leitura atual dos sensores e ultima tecla de movimentacao pressionada
     Retorno: posicao dos servomotores
    *************************************************************************/
    servosPositions adjustPositions(int operationMode, sensorsReading sensorsReading, char lastMovementKey);

    private:
    /************************************************************************
     limitPositions
     Logica para limitar a posicao dos servos
     Parametros de entrada: nenhum
     Retorno: nenhum
    *************************************************************************/
    void limitPositions();

    /************************************************************************
     automatic
     Logica de movimentacao automatica da placa
     Parametros de entrada: leitura dos sensores
     Retorno: posicao dos servomotores
    *************************************************************************/
    void automatic(sensorsReading sensorsReading);

    /************************************************************************
     manual
     Logica de movimentacao manual da placa
     Parametros de entrada: leitura do teclado
     Retorno: posicao dos servomotores
    *************************************************************************/
    void manual(char key);

    // atributos
    int servoHorizontal;
    int servoVertical;
    servosPositions positions;
};

extern ControllerClass Controller;

#endif // CONTROLLER_H_INCLUDED