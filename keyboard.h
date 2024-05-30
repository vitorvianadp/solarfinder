#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#define NO_INFO -1

/************************
 keyboard_debug
 Funcao usada so para debugar, remover dps
 entradas
   texto : texto a ser enviado no serial
 saidas
   nenhuma
*************************/
extern void keyboard_debug(char* texto, int ln = 0);
extern void keyboard_debug(int value, int ln = 0);

/************************
 keyboard_getKeys
 Obtem tecla de movimentacao no caso manual
 Obtem status do switch para determinar o modo
 entradas
   nenhuma
 saidas
   informacao recebida na interface ou NO_INFO
*************************/
extern char* keyboard_getKeys();

#endif // KEYBOARD_H_INCLUDED
