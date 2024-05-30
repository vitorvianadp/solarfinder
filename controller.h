#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include "config.h"

// Defines
#define SERVO_HOR_MAX 175 // limite superior
#define SERVO_HOR_MIN 5 // limite inferior
// 65 graus no maximo

#define SERVO_VERT_MAX 60
#define SERVO_VERT_MIN 1

// Funcoes

/************************************************************************
 controller_limitPositions
 Logica para limitar a posicao dos servos
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
extern void controller_limitPositions();


/************************************************************************
 controller_automatic
 Logica de movimentacao automatica da placa
 Parametros de entrada: leitura dos sensores
 Retorno: posicao dos servomotores
*************************************************************************/
extern void controller_automatic(sensorsReading sensorsReading);

/************************************************************************
 controller_manual
 Logica de movimentacao manual da placa
 Parametros de entrada: leitura do teclado
 Retorno: posicao dos servomotores
*************************************************************************/
extern void controller_manual(char key);

/************************************************************************
 controller_adjustPositions
 Ajusta as posicoes dos servos motores com base na logica de controle
 Parametros de entrada: modo de operacao, leitura atual dos sensores e ultima tecla de movimentacao pressionada
 Retorno: posicao dos servomotores
*************************************************************************/
extern servosPositions controller_adjustPositions(int operationMode, sensorsReading sensorsReading, char lastMovementKey);

#endif // CONTROLLER_H_INCLUDED