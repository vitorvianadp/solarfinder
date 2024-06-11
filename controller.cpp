#include "controller.h"
#include "config.h"

ControllerClass Controller;

/************************************************************************
 construtor
*************************************************************************/
ControllerClass::ControllerClass()
{
    servoHorizontal = SERVO_HOR_INITIAL;
    servoVertical = SERVO_VERT_INITIAL;
}

/************************************************************************
 limitPositions
 Logica para limitar a posicao dos servos
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void ControllerClass::limitPositions(){
    if (servoVertical > SERVO_VERT_MAX) //se atingir o limite do servo, servo não se move
        servoVertical = SERVO_VERT_MAX;
    else if (servoVertical < SERVO_VERT_MIN) //se atingir o limite do servo, servo não se move
        servoVertical = SERVO_VERT_MIN;
    if (servoHorizontal < SERVO_HOR_MIN)
        servoHorizontal = SERVO_HOR_MIN;
    else if (servoHorizontal > SERVO_HOR_MAX)
        servoHorizontal = SERVO_HOR_MAX;
}

/************************************************************************
 controller_automatic
 Logica de movimentacao automatica da placa
 Parametros de entrada: leitura dos sensores
 Retorno: posicao dos servomotores
*************************************************************************/
void ControllerClass::automatic(sensorsReading luminosities){

    int lt = luminosities.values[0]; // superior esquerdo
    int rt = luminosities.values[1]; // superior direito
    int ld = luminosities.values[2]; // inferior esquerdo
    int rd = luminosities.values[3]; // inferior direito

    // Calculo de dtime (delay time) e tol (tolerancia) usando os potenciometros
    int dtime = 3; //analogRead(POTENCIOMETRO_TEMPO_PIN)/20;
    int tol = 100; //analogRead(POTENCIOMETRO_TOLER_PIN);
    #ifdef DEBUG
        tol = 0;
    #endif
    // Leitura analogica do potenciometro vai de 0 a 1023, entao pode-se dividir ou multiplicar esse valor para obter outras faixas

    // Calculo das medias das leituras
    int avt = (lt + rt) / 2; // media nos sensores de cima
    int avd = (ld + rd) / 2; // media nos sensores embaixo
    int avl = (lt + ld) / 2; // media nos sensores da esquerda
    int avr = (rt + rd) / 2; // media nos sensores da direita
    int dvert = avt - avd; // diferença para a vertical
    int dhoriz = avl - avr;// diferença para a horizontal

    // Controle de posicionamento
    if (-1*tol > dvert || dvert > tol) // se a diferença entre os valores for maior que a tolerância, movimentar
    {
        if (avt > avd) // se o valor medio superior for maior que o inferior, movimenta para cima
            servoVertical-=2;
        else if (avt < avd) // se o valor medio inferior for maior que o superior, movimenta para baixo
            servoVertical+=2;
    }



    if (-1*tol > dhoriz || dhoriz > tol) //mesmo procedimento, mas para o horizontal
    {
        if (avl > avr)
            servoHorizontal-=2;
        else if (avl < avr)
            servoHorizontal+=2;
        else // avl = avr
            delay(5000);
    }

    // #ifdef DEBUG
    //     Serial.print("avt = ");
    //     Serial.println(avt);
    //     Serial.print("avd = ");
    //     Serial.println(avd);
    //     Serial.print("avl = ");
    //     Serial.println(avl);
    //     Serial.print("avr = ");
    //     Serial.println(avr);
    //     Serial.print("dvert = ");
    //     Serial.println(dvert);
    //     Serial.print("dhoriz = ");
    //     Serial.println(dhoriz);

    //     Serial.print("delay dtime =");
    //     Serial.println(dtime);

    //     Serial.print("tolerancia = ");
    //     Serial.println(tol);
    //     // Serial.print e Serial.println adicionam um delay grande (servo vai se mover mais devagar)
    // #endif

    //delay(dtime);
}

/************************************************************************
 controller_manual
 Logica de movimentacao manual da placa
 Parametros de entrada: leitura do teclado
 Retorno: posicao dos servomotores
*************************************************************************/
void ControllerClass::manual(char key){
    if (key == 'u')
        servoVertical-=2;
    else if (key == 'd')
        servoVertical+=2;
    else if (key == 'r')
        servoHorizontal+=2;
    else if (key == 'l')
        servoHorizontal-=2;
}

/************************************************************************
 controller_adjustPositions
 Ajusta as posicoes dos servos motores com base na logica de controle
 Parametros de entrada: modo de operacao, leitura atual dos sensores e ultima tecla de movimentacao pressionada
 Retorno: posicao dos servomotores
*************************************************************************/
servosPositions ControllerClass::adjustPositions(int operationMode, sensorsReading luminosities, char lastMovementKey){

    if (operationMode == MANUAL){
        manual(lastMovementKey);
    }
    else if (operationMode == AUTOMATIC){
        automatic(luminosities);
    }

    limitPositions();
    positions.vertical = servoVertical;
    positions.horizontal = servoHorizontal;

    return positions;
}