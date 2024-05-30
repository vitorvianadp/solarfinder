#ifndef POSITIONER_H_INCLUDED
#define POSITIONER_H_INCLUDED

#include "config.h"

/************************************************************************
 positioner_init
 Inicia os servomotores
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
extern void positioner_init();

/************************************************************************
 positioner_moveMotors
 Movimenta os motores
 Parametros de entrada: valores correspondentes aos servos
 Retorno: nenhum
*************************************************************************/
extern void positioner_moveMotors(servosPositions servosPositions);

#endif // POSITIONER_H_INCLUDED