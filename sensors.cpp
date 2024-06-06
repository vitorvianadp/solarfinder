#include <Arduino.h>

#include "config.h"
#include "sensors.h"

SensorsClass Sensors;

/************************************************************************
 construtor
*************************************************************************/
SensorsClass::SensorsClass()
{
}

/************************
 getLuminosity
 Computa os valores de luminosidade detectada nos 4 LDRs
 entradas
   nenhuma
 saidas
   struct com as 4 leituras analogicas atuais dos sensores
*************************/
sensorsReading SensorsClass::getLuminosity(){
    sensorsReading luminosities;

    #ifdef DEBUG
        luminosities.values[0] = analogRead(LDRLT_PIN);
        luminosities.values[1] = 550;
        luminosities.values[2] = 650;
        luminosities.values[3] = 750;
    #endif

    //leitura dos sensores LDR
    luminosities.values[0] = analogRead(LDRLT_PIN); // superior esquerdo
    luminosities.values[1] = 0;
    luminosities.values[2] = 0;
    luminosities.values[3] = 0;
    // luminosities.values[1] = analogRead(LDRRT_PIN); // superior direito
    // luminosities.values[2] = analogRead(LDRLD_PIN); // inferior esquerdo
    // luminosities.values[3] = analogRead(LDRRD_PIN); // inferior direito

    return luminosities;
}