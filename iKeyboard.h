#ifndef IKEYBOARD_H_INCLUDED
#define IKEYBOARD_H_INCLUDED

#include "config.h"

class KeyboardInterface {
    public:
    /************************
     debug
     Funcao usada so para debugar, remover dps
     entradas
       texto : texto a ser enviado no serial
     saidas
       nenhuma
    *************************/
    virtual void debug(char* texto, int ln = 1) = 0;
    virtual void debug(int value, int ln = 1) = 0; // override

    /************************
     getKeys
     Obtem tecla de movimentacao no caso manual
     Obtem status do switch para determinar o modo
     entradas
       nenhuma
     saidas
       informacao recebida na interface
    *************************/
    virtual char* getKeys() = 0;
};

#endif // IKEYBOARD_H_INCLUDED