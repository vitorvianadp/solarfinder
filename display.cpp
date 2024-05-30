#include <Arduino.h>

#include "config.h"
#include "display.h"

#define DEBUG

/************************
 display_getInfo
 Obtem dados dos sensores para exibir na interface
 entradas
   dados: vetor com as leituras dos 4 sensores de luminosidade
 saidas
   nenhuma
*************************/
void display_getInfo(int values[]){
    #ifdef DEBUG
        Serial.println("Leituras:");
        for (int i = 0; i < 4; i++){
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(values[i]);
        }
    #endif
    // implementar POST request para interface com os dados do sensor
}