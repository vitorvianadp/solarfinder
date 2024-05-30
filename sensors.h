#ifndef SENSORS_H_INCLUDED
#define SENSORS_H_INCLUDED

#include "config.h"

/************************
 sensors_getLuminosity
 Computa os valores de luminosidade detectada nos 4 LDRs
 entradas
   nenhuma
 saidas
   vetor com as 4 leituras analogicas atuais dos sensores
*************************/
extern sensorsReading sensors_getLuminosity();

#endif // SENSORS_H_INCLUDED