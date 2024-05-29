#ifndef MOVEMENT_H_INCLUDED
#define MOVEMENT_H_INCLUDED

#include <servo.h>


#define ldrlt A0 //LDR top left - BOTTOM LEFT <--- BDG
#define ldrrt A2 //LDR top rigt - BOTTOM RIGHT
#define ldrld A1 //LDR down left - TOP LEFT
#define ldrrd A3 //ldr down rigt - TOP RIGHT

//Definindo pinos dos servos
#define servo_vert_pin 10
#define servo_hor_pin 9

//Definindo pinos dos potenciômetros

#define potenciometro_tempo A4 //potenciômetro para ajuste temporal
#define potenciometro_tol A5   // potenciômetro para ajuste de tolerância


Servo horizontal; // horizontal servo in DIGITAL PIN 9
int servoh = 100; 
int servohLimitHigh = 175;
int servohLimitLow = 5;
// 65 degrees MAX

Servo vertical; // vertical servo in DIGITAL PIN 10
int servov = 45; 
int servovLimitHigh = 60;
int servovLimitLow = 1;

void servos_init(void);

void automatic_movement(void);

void manual_movement(char tecla);


#endif