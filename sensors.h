#ifndef SENSORS_H_INCLUDED
#define SENSORS_H_INCLUDED

#include "config.h"
#include "iSensors.h"

class SensorsClass: public SensorsInterface{
    public:
    /************************************************************************
     construtor
    *************************************************************************/
    SensorsClass();

    /************************
     getLuminosity
     Computa os valores de luminosidade detectada nos 4 LDRs
     entradas
     nenhuma
     saidas
     vetor com as 4 leituras analogicas atuais dos sensores
    *************************/
    sensorsReading getLuminosity();
};

extern SensorsClass Sensors;

#endif // SENSORS_H_INCLUDED