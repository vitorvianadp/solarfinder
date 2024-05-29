#include <Servo.h> 
#include "movement.h"


void servos_init(void){


// definir pinos dos servos na bibliotca servo.h
horizontal.attach(servo_hor_pin);
vertical.attach(servo_vert_pin);

horizontal.write(servoh); //escreve o valor inicial de movimentação do servo na horizontal
vertical.write(servov);  //escreve o valor inicial de movimentação do servo na vertical 

}


void automatic_movement(void){

//leitura dos sensores LDR

int lt = analogRead(ldrlt); // top left
int rt = analogRead(ldrrt); // top right
int ld = analogRead(ldrld); // down left
int rd = analogRead(ldrrd); // down right


//int dtime = 10; int tol = 90;
  
// É possível calcular dtime (delaty time) e tol (tolerancia) usando os potenciômetros:
int dtime = analogRead(potenciometro_tempo)/20;
int tol = analogRead(potenciometro_tol);
//Leitura analógica do potênciometro vai de 0 a 1023, então pode-se dividir ou multiplicar esse valor para obter outras faixas
 
//Cálculo das médias das leituras
int avt = (lt + rt) / 2; // average value top
int avd = (ld + rd) / 2; // average value down
int avl = (lt + ld) / 2; // average value left
int avr = (rt + rd) / 2; // average value right
int dvert = avt - avd; // diferença para a vertical
int dhoriz = avl - avr;// diferença para a horizontal


// Controle de posicionamento

if (-1*tol > dvert || dvert > tol) // se a diferença entre os valores for maior que a tolerância, movimentar 
 {
 if (avt > avd) // se o valor medio superior for maior que o inferior, movimenta para cima
 {
 servov = ++servov;
 if (servov > servovLimitHigh) //se atingir o limite do servo, servo não se move
 {servov = servovLimitHigh;}
 }
 else if (avt < avd) // se o valor medio inferior for maior que o superior, movimenta para baixo
 {servov= --servov;
 if (servov < servovLimitLow) //se atingir o limite do servo, servo não se move
 { servov = servovLimitLow;}
 }
 vertical.write(servov); //movimentação do servo
 }
  
  
  
if (-1*tol > dhoriz || dhoriz > tol) //mesmo procedimento, mas para o horizontal
 {
 if (avl > avr)
 {
 servoh = --servoh;
 if (servoh < servohLimitLow)
 {
 servoh = servohLimitLow;
 }
 }
 else if (avl < avr)
 {
 servoh = ++servoh;
 if (servoh > servohLimitHigh)
 {
 servoh = servohLimitHigh;
 }
 }
 else if (avl = avr)
 { delay(5000); 
 }
 horizontal.write(servoh);
 }
  
// Serial.print("avt = ");
// Serial.println(avt);
// Serial.print("avd = ");
// Serial.println(avd);
// Serial.print("avl = ");
// Serial.println(avl);
// Serial.print("avr = ");
// Serial.println(avr);
  
//  Serial.print("delay dtime =");
//  Serial.println(dtime);
 
//  Serial.print("tolerancia = ");
//  Serial.println(tol);
  

// Serial.print e Serial.println adicionam um delay grande (servo vai se mover mais devagar)

 delay(dtime);

}

void manual_movement(char tecla){
    if (tecla == 'U'){
        servov = ++servov;
         if (servov > servovLimitHigh) //se atingir o limite do servo, servo não se move
            {servov = servovLimitHigh;}

        }

    else if (tecla == 'D'){
        servov = --servov;
         if (servov < servovLimitLow) //se atingir o limite do servo, servo não se move
            { servov = servovLimitLow;}
        }
    

    else if (tecla == 'R'){
        servoh = ++servoh;
        if (servoh > servohLimitHigh)
            {servoh = servohLimitHigh;}
        }

    else if (tecla == 'L'){
        servoh = --servoh;
        if (servoh < servohLimitLow)
            {servoh = servohLimitLow;}
            
        }
}