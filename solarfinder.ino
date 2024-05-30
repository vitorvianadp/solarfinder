#include <Arduino.h>

/*
    PLACA SOLAR INTELIGENTE
*/

#include "config.h"
#include "display.h"
#include "keyboard.h"
#include "sensors.h"

int eventCode;
int actionCode;
int state;
int internEvent;
stateTransitionMatrix stateTransition;

/************************************************************************
 executeAction
 Executa uma acao
 Parametros de entrada:
    (int) codigo da acao a ser executada
 Retorno: (int) codigo do evento interno ou NO_EVENTS
*************************************************************************/
/*
TODO: implementar as funcoes, por enquanto ta com os prints para testes
*/
int executeAction(int actionCode)
{
    int retval;

    retval = NO_EVENTS;
    if (actionCode == NO_ACTIONS)
        return retval;

    switch(actionCode)
    {
    case A01:
        keyboard_debug("Modo manual selecionado.", 1);
        break;
    case A02:
        keyboard_debug("Modo automatico selecionado.", 1);
        break;
    case A03:
        keyboard_debug("Entrada de teclas para posicionar a placa.", 1);
        retval = MOVE_MOTORS;
        break;
    case A04:
        // a principio uma acao so, a funcao seria chamada aqui e deveria tratar la dentro como a movimentacao deve ser feita
        keyboard_debug("Movimentacao dos motores.", 1);
        break;
    case A05:
        keyboard_debug("Leitura de luminosidade dos sensores.", 1);
        display_getInfo(sensors_getLuminosity());
        retval = MOVE_MOTORS;
        break;
    case A06:
        // funcao deve ser chamada aqui e tratar qual deve ser o novo modo de operacao
        keyboard_debug("Modo de operacao alterado.", 1);
        break;
    case A07:
        keyboard_debug("Placa agora esta travada na posicao definida.", 1);
        break;
    } // switch

    return retval;
} // executeAction

/************************************************************************
 initStateMachine
 Carrega a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void initStateMachine()
{

    // inicializacao da matriz de transicao de estado
    for (int i = 0; i < NUM_STATES; i++) {
        for (int j = 0; j < NUM_EVENTS; j++) {
        stateTransition.action[i][j] = NO_ACTIONS; // nenhuma acao associada a nenhuma transicao inicialmente
        stateTransition.next_state[i][j] = i; // transicao leva para o mesmo estado se nao alterar nada
        }
    }

    stateTransition.next_state[IDLE_CLIENT][SELECT_MANUAL] = MANUAL_OPERATION;
    stateTransition.action[IDLE_CLIENT][SELECT_MANUAL] = A01;

    stateTransition.next_state[IDLE_CLIENT][SELECT_AUTOMATIC] = AUTOMATIC_OPERATION;
    stateTransition.action[IDLE_CLIENT][SELECT_AUTOMATIC] = A02;

    stateTransition.next_state[MANUAL_OPERATION][INPUT_KEYS] = MANUAL_DATA_PROCESSING;
    stateTransition.action[MANUAL_OPERATION][INPUT_KEYS] = A03;

    stateTransition.next_state[MANUAL_DATA_PROCESSING][MOVE_MOTORS] = MANUAL_OPERATION;
    stateTransition.action[MANUAL_DATA_PROCESSING][MOVE_MOTORS] = A04;

    stateTransition.next_state[MANUAL_OPERATION][SWITCH_MODE] = AUTOMATIC_OPERATION;
    stateTransition.action[MANUAL_OPERATION][SWITCH_MODE] = A06;

    stateTransition.next_state[AUTOMATIC_OPERATION][INPUT_SENSORS] = AUTOMATIC_DATA_PROCESSING;
    stateTransition.action[AUTOMATIC_OPERATION][INPUT_SENSORS] = A05;

    stateTransition.next_state[AUTOMATIC_DATA_PROCESSING][MOVE_MOTORS] = AUTOMATIC_OPERATION;
    stateTransition.action[AUTOMATIC_DATA_PROCESSING][MOVE_MOTORS] = A04;

    stateTransition.next_state[AUTOMATIC_OPERATION][SWITCH_MODE] = MANUAL_OPERATION;
    stateTransition.action[AUTOMATIC_OPERATION][SWITCH_MODE] = A06;

    stateTransition.next_state[MANUAL_OPERATION][SAVE_POSITION] = IDLE_CLIENT;
    stateTransition.action[MANUAL_OPERATION][SAVE_POSITION] = A07;

} // initStateMachine

/************************************************************************
 initSystem
 Inicia o sistema ...
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/

// se precisar inicializar algo junto com a maquina de estados vai aqui
void initSystem()
{
   initStateMachine();
   state = IDLE_CLIENT;
   internEvent = NO_EVENTS;
} // initSystem

/************************************************************************
 obtainEvent
 Obtem um evento, que pode ser da IHM ou do sensor
 Parametros de entrada: nenhum
 Retorno: codigo do evento
*************************************************************************/
/*
TODO: implementar as condicoes para IHM real, adicionando header e source para os componentes do diagrama
*/

int obtainEvent()
{
    int event = NO_EVENTS;
    char code;
    code = keyboard_getKeys()[0];

    /*
    evento:                        codigo   implementacao (onde recebe o codigo ou como ativar)
    inicializar e escolher manual: 'm'      keyboard
    inicializar e escolher auto:   'a'      keyboard
    apertar setas para movimentar: 'k'      keyboard
    movimentar os motores:         'r'      evento interno apos executar A03 ou A05
    leitura dos sensores LDR:      'l'      task continua se estiver no modo automatico
    mudar modo com o switch:       's'      keyboard
    salvar posicao atual:          'p'      keyboard
    */

    switch (code)
    {
    case 'm':
        event = SELECT_MANUAL;
        break;
    case 'a':
        event = SELECT_AUTOMATIC;
        break;
    case 'k': // pensar ainda em como fazer a logica de movimentacao para cada lado, talvez usar keys[1]
        event = INPUT_KEYS;
        break;
    case 'r': // dentro da propia funcao para movimentacao que deve ser checado o modo de operacao
        event = MOVE_MOTORS;
        break;
    case 'l':
        event = INPUT_SENSORS;
        break;
    case 's': // switch alterado -> tratamento deve ser feito dentro de keyboard.cpp msm
        event = SWITCH_MODE;
        break;
    case 'p':
        event = SAVE_POSITION;
        break;
    default:
        break;
    }

    return event;

} // obtainEvent

/************************************************************************
 obtainAction
 Obtem uma acao da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo da acao
*************************************************************************/
int obtainAction(int state, int eventCode) {
  return stateTransition.action[state][eventCode];
} // obtainAction

/************************************************************************
 obtainNextState
 Obtem o proximo estado da Matriz de transicao de estados
 Parametros de entrada: estado (int)
                        evento (int)
 Retorno: codigo do estado
*************************************************************************/
int obtainNextState(int state, int eventCode) {
  return stateTransition.next_state[state][eventCode];
} // obtainNextState

/************************************************************************
 Main
 Loop principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void setup() {
  Serial.begin(9600);

  initSystem();
  Serial.println("Sistema iniciado");
} // setup

void loop() {
    if (internEvent == NO_EVENTS) {
        eventCode = obtainEvent();
    } else {
        eventCode = internEvent;
    }
    if (eventCode != NO_EVENTS)
    {
        actionCode = obtainAction(state, eventCode);
        state = obtainNextState(state, eventCode);
        internEvent = executeAction(actionCode);

        keyboard_debug("Estado: ");
        keyboard_debug(state);
        keyboard_debug(" Evento: ");
        keyboard_debug(eventCode);
        keyboard_debug(" Acao: ");
        keyboard_debug(actionCode, 1);
  }
} // loop