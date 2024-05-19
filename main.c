#include <stdio.h>

/*
    PLACA SOLAR INTELIGENTE
*/

#include "config.h"

int eventCode;
int actionCode;
int state;
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
        printf("Modo manual selecionado.\n");
        break;
    case A02:
        printf("Modo automatico selecionado.\n");
        break;
    case A03:
        printf("Entrada de teclas para posicionar a placa.\n");
        break;
    case A04:
        // a principio uma acao so, a funcao seria chamada aqui e deveria tratar la dentro como a movimentacao deve ser feita
        printf("Movimentacao dos motores.\n");
        break;
    case A05:
        printf("Leitura de luminosidade dos sensores.\n");
        break;
    case A06:
        // funcao deve ser chamada aqui e tratar qual deve ser o novo modo de operacao
        printf("Modo de operacao alterado.\n");
        break;
    case A07:
        printf("Placa agora esta travada na posicao definida.\n");
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
char* keys;
char buf[10];

char* ihm_readKeys()
{
    printf("obter teclas:");
    scanf("%s", buf);
    return buf;
}

int decodeSelectManual()
{
    if (keys[0] == 'm') // modo manual
    {
        return true;
    }
    return false;
}// decodeSelectManual

int decodeSelectAutomatic()
{
    if (keys[0] == 'a')
    {
        return true;
    }
    return false;
}// decodeSelectAutomatic

int decodeInputKeys()
{
    if (keys[0] == 'k') // movimentacao pelo teclado
    {
        return true;
    }
    return false;
}// decodeInputKeys

int decodeMoveMotors()
{
    if (keys[0] == 'r') // rotacao da placa
    {
        return true;
    }
    return false;
}// decodeMoveMotors

int decodeInputSensors()
{
    if (keys[0] == 'l') // luminosidade nos sensores
    {
        return true;
    }
    return false;
}// decodeInputSensors

int decodeSwitchMode()
{
    if (keys[0] == 's') // troca de modo
    {
        return true;
    }
    return false;
}// decodeSwitchMode

int decodeSavePosition()
{
    if (keys[0] == 'p') // salva posicao atual da placa
    {
        return true;
    }
    return false;
}// decodeSavePosition

int obtainEvent()
{
    int retval = NO_EVENTS;

    keys = ihm_readKeys();
    if (decodeSelectManual())
        return SELECT_MANUAL;
    if (decodeSelectAutomatic())
        return SELECT_AUTOMATIC;
    if (decodeInputKeys())
        return INPUT_KEYS;
    if (decodeMoveMotors())
        return MOVE_MOTORS;
    if (decodeInputSensors())
        return INPUT_SENSORS;
    if (decodeSwitchMode())
        return SWITCH_MODE;
    if (decodeSavePosition())
        return SAVE_POSITION;

    return retval;

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
int main() {

  int eventCode;
  int actionCode;
  int state;
  int internEvent;

  state = IDLE_CLIENT;
  internEvent = NO_EVENTS;

  initSystem();
  printf("Sistema iniciado.\n");
  while (true) {
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
       printf("Estado: %d Evento: %d Acao:%d\n", state, eventCode, actionCode);
    }
  } // while true
} // main