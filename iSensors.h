#ifndef ISENSORS_H_INCLUDED
#define ISENSORS_H_INCLUDED

#include "config.h"

class SensorsInterface {
    public:
    /************************
     sensors_getLuminosity
     Computa os valores de luminosidade detectada nos 4 LDRs
     entradas
     nenhuma
     saidas
     vetor com as 4 leituras analogicas atuais dos sensores
    *************************/
    virtual sensorsReading getLuminosity() = 0;
};

#endif // ISENSORS_H_INCLUDED