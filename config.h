#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define DEBUG

#define true  1
#define false 0

#define NUM_STATES 5
#define NUM_EVENTS 7
#define NUM_SENSORS 4

#define NO_EVENTS -1 // Nenhum evento
#define NO_ACTIONS -1 // Nenhuma acao

// ESTADOS
enum states { IDLE_CLIENT, MANUAL_OPERATION, AUTOMATIC_OPERATION, MANUAL_DATA_PROCESSING, AUTOMATIC_DATA_PROCESSING };

// EVENTOS
enum events { SELECT_MANUAL, SELECT_AUTOMATIC, INPUT_KEYS, INPUT_SENSORS, MOVE_MOTORS, SWITCH_MODE, SAVE_POSITION };

// ACOES
enum actions { A01, A02, A03, A04, A05, A06, A07 };

// MATRIZ DE TRANSICAO
typedef struct stateTransitionMatrix {
    int action[NUM_STATES][NUM_EVENTS];
    int next_state[NUM_STATES][NUM_EVENTS];
} stateTransitionMatrix;

// VETOR DE LEITURAS DOS SENSORES
typedef struct sensorsReading {
    int luminosityValues[NUM_SENSORS];
} sensorsReading;

#endif // CONFIG_H_INCLUDED