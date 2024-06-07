#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "config.h"
#include "iDisplay.h"

class DisplayClass: public DisplayInterface{
    public:
    /************************************************************************
     construtor
    *************************************************************************/
    DisplayClass();

    /************************
     getInfo
     Obtem dados dos sensores para exibir na interface
     entradas
      dados: vetor com as leituras dos 4 sensores de luminosidade
     saidas
      nenhuma
    *************************/
    void getInfo(sensorsReading sensorsReading);
    bool isActive();
    void activate(bool command);

    private:
    bool active;
};

extern DisplayClass Display;

#endif // DISPLAY_H_INCLUDED