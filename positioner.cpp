#include "positioner.h"
#include "config.h"

#ifndef DEBUG
    #include <Servo.h>
    Servo horizontal; // servo horizontal no pino 9
    Servo vertical; // servo vertical no pino 10
#endif

/************************************************************************
 positioner_init
 Inicia os servomotores
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void positioner_init(void){
    // definir pinos dos servos na biblioteca servo.h
    #ifndef DEBUG
        horizontal.attach(SERVO_HOR_PIN);
        vertical.attach(SERVO_VERT_PIN);

        horizontal.write(SERVO_HOR_INITIAL); //escreve o valor inicial de movimentação do servo na horizontal
        vertical.write(SERVO_VERT_INITIAL);  //escreve o valor inicial de movimentação do servo na vertical
    #endif
}

/************************************************************************
 positioner_moveMotors
 Movimenta os motores
 Parametros de entrada: valores correspondentes aos servos
 Retorno: nenhum
*************************************************************************/
void positioner_moveMotors(servosPositions servosPositions){

    #ifndef DEBUG
        vertical.write(servosPositions.vertical); //movimentação do servo
        horizontal.write(servosPositions.horizontal);
    #else
        Serial.print("Servo vertical: ");
        Serial.println(servosPositions.vertical);
        Serial.print("Servo horizontal: ");
        Serial.println(servosPositions.horizontal);
    #endif

}