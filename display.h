#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "config.h"

/************************
 display_getInfo
 Obtem dados dos sensores para exibir na interface
 entradas
   dados: vetor com as leituras dos 4 sensores de luminosidade
 saidas
   nenhuma
*************************/
extern void display_getInfo(sensorsReading sensorsReading);

#endif // DISPLAY_H_INCLUDED