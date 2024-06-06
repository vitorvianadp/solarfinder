#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include "iKeyboard.h"

class KeyboardClass: public KeyboardInterface{
    public:
    /************************************************************************
     construtor
    *************************************************************************/
    KeyboardClass();

    /************************
     debug
     Funcao usada so para debugar, remover dps
     entradas
       texto : texto a ser enviado no serial
     saidas
       nenhuma
    *************************/
    void debug(char* texto, int ln);
    void debug(int value, int ln); // override

    /************************
     getKeys
     Obtem tecla de movimentacao no caso manual
     Obtem status do switch para determinar o modo
     entradas
       nenhuma
     saidas
       informacao recebida na interface
    *************************/
    char* getKeys();
    void init(); 

    private:
    char buf[10];
};

extern KeyboardClass Keyboard;

#endif // KEYBOARD_H_INCLUDED