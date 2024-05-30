#ifndef IDISPLAY_H_INCLUDED
#define IDISPLAY_H_INCLUDED

#include "config.h"

class DisplayInterface {
    public:
    /************************
     getInfo
     Obtem dados dos sensores para exibir na interface
     entradas
      dados: vetor com as leituras dos 4 sensores de luminosidade
     saidas
      nenhuma
    *************************/
    virtual void getInfo(sensorsReading sensorsReading) = 0;
};

#endif // IDISPLAY_H_INCLUDED