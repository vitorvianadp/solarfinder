#include <Arduino.h>

#include "config.h"
#include "keyboard.h"

#define DEBUG

/************************
 keyboard_debug
 Funcao usada so para debugar, remover dps
 entradas
   texto : texto a ser enviado no serial
 saidas
   nenhuma
*************************/
void keyboard_debug(char *texto, int ln = 0)
{
    #ifdef DEBUG
        if (ln == 0)
            Serial.print(texto);
        else
            Serial.println(texto);
    #endif
}

void keyboard_debug(int value, int ln = 0)
{
    #ifdef DEBUG
        if (ln == 0)
            Serial.print(value);
        else
            Serial.println(value);
    #endif
}

/************************
 keyboard_getKeys
 Obtem tecla de movimentacao no caso manual
 Obtem status do switch para determinar o modo
 entradas
   nenhuma
 saidas
   informacao recebida na interface ou NO_INFO
*************************/
char buf[10];
char *keyboard_getKeys()
{
    // nessa funcao pode ser feita talvez uma conversao do evento de apertar o switch ou apertar tecla de movimentacao
    // para o codigo correspondente na logica de movimentacao (meio que a logica de tratamento fica aqui e de movimentacao nao fica aqui)

    /*
    evento:                        codigo
    inicializar e escolher manual: 'm'
    inicializar e escolher auto:   'a'
    apertar setas para movimentar: 'k'
    mudar modo com o switch:       's'
    salvar posicao atual:          'p'
    */

    int read_count = 0;

    if (Serial.available() > 0)
    {
        read_count = Serial.readBytesUntil('\n', buf, sizeof(buf) / sizeof(buf[0]) - 1);
    }

    buf[read_count] = '\0';
    if (read_count > 0)
    {
        Serial.println(buf);
    }

    return buf;
}