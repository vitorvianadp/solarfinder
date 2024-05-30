#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <Arduino.h>

#define DEBUG

#define true  1
#define false 0

#define NUM_STATES 5
#define NUM_EVENTS 7
#define NUM_SENSORS 4

#define NO_EVENTS -1 // Nenhum evento
#define NO_ACTIONS -1 // Nenhuma acao

// PINOS - AJUSTAR PARA ESP
// ANALOGICOS

// SENSORES
#define LDRLT_PIN A0 //LDR superior esquerdo
#define LDRRT_PIN A2 //LDR superior direito
#define LDRLD_PIN A1 //LDR inferior esquerdo
#define LDRRD_PIN A3 //LDR inferior direito
//POTENCIOMETROS
#define POTENCIOMETRO_TEMPO_PIN A4 //potenciômetro para ajuste temporal
#define POTENCIOMETRO_TOLER_PIN A5   // potenciômetro para ajuste de tolerância

//DIGITAIS

// SERVOS
#define SERVO_VERT_PIN 10
#define SERVO_HOR_PIN 9

// POSICOES INICIAIS DOS SERVOS
#define SERVO_HOR_INITIAL 100
#define SERVO_VERT_INITIAL 45

// ESTADOS
enum states { IDLE_CLIENT, MANUAL_OPERATION, AUTOMATIC_OPERATION, MANUAL_DATA_PROCESSING, AUTOMATIC_DATA_PROCESSING };

// EVENTOS
enum events { SELECT_MANUAL, SELECT_AUTOMATIC, INPUT_KEYS, INPUT_SENSORS, MOVE_MOTORS, SWITCH_MODE, SAVE_POSITION };

// ACOES
enum actions { A01, A02, A03, A04, A05, A06, A07 };

// MODOS DE OPERACAO
enum mode { NO_MODE = -1, MANUAL, AUTOMATIC };

// MATRIZ DE TRANSICAO
typedef struct stateTransitionMatrix {
    int action[NUM_STATES][NUM_EVENTS];
    int next_state[NUM_STATES][NUM_EVENTS];
} stateTransitionMatrix;

// VETOR DE LEITURAS DOS SENSORES
typedef struct sensorsReading {
    int values[NUM_SENSORS];
} sensorsReading;

// POSICAO DOS SERVOS
typedef struct servosPositions {
    int horizontal = SERVO_HOR_INITIAL;
    int vertical = SERVO_VERT_INITIAL;
} servosPositions;

#endif // CONFIG_H_INCLUDED