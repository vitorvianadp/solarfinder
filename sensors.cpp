#include <Arduino.h>

#include "config.h"
#include "display.h"

/************************
 sensors_getLuminosity
 Computa os valores de luminosidade detectada nos 4 LDRs
 entradas
   nenhuma
 saidas
   struct com as 4 leituras analogicas atuais dos sensores
*************************/
sensorsReading sensors_getLuminosity(){
    sensorsReading sensorsReading;
    #ifdef DEBUG
        sensorsReading.luminosityValues[0] = 450;
        sensorsReading.luminosityValues[1] = 550;
        sensorsReading.luminosityValues[2] = 650;
        sensorsReading.luminosityValues[3] = 750;
    #endif
    // adicionar leitura dos sensores de fato (comunicacao do esp)
    return sensorsReading;
}