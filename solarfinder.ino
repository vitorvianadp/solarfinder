//#include <Arduino_FreeRTOS.h>
//#include <queue.h>
//#include <semphr.h>

/*
    PLACA SOLAR INTELIGENTE
*/

#include "config.h"
#include "controller.h"
#include "display.h"
#include "keyboard.h"
#include "positioner.h"
#include "sensors.h"

#define TASK1_INTERVAL 150
#define TASK2_INTERVAL 100

int actionCode;
int state;
int internEvent;
int operationMode = NO_MODE;
char lastMovementKey;
stateTransitionMatrix stateTransition;

/***********************************************************************
 FreeRTOS
 ***********************************************************************/
void taskStateMachine(void *pvParameters);
void taskObtainEvent(void *pvParameters);
QueueHandle_t xQueue;

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
        Keyboard.debug((char*)"Modo manual selecionado.",1);
        operationMode = MANUAL;
        break;
    case A02:
        Keyboard.debug((char*)"Modo automatico selecionado.",1);
        operationMode = AUTOMATIC;
        break;
    case A03:
        Keyboard.debug((char*)"Entrada de teclas para posicionar a placa.",1);
        retval = MOVE_MOTORS;
        break;
    case A04:
        // a principio uma acao so, a funcao seria chamada aqui e deveria tratar la dentro como a movimentacao deve ser feita
        Keyboard.debug((char*)"Movimentacao dos motores.",1);
        Positioner.moveMotors(Controller.adjustPositions(operationMode, Sensors.getLuminosity(), lastMovementKey));
        break;
    case A05:
        Keyboard.debug((char*)"Leitura de luminosidade dos sensores.",1);
        Display.getInfo(Sensors.getLuminosity());
        retval = MOVE_MOTORS;
        break;
    case A06:
        // funcao deve ser chamada aqui e tratar qual deve ser o novo modo de operacao
        Keyboard.debug((char*)"Modo de operacao alterado.",1);
        operationMode = 1 - operationMode; // switch
        break;
    case A07:
        Keyboard.debug((char*)"Placa agora esta travada na posicao definida.",1);
        operationMode = NO_MODE;
        break;
    default:
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
 TASKS
*************************************************************************/

/************************************************************************
 taskObtainEvent
 Tarefa para obter um evento, que pode ser da IHM ou do sensor e colocar na fila
 Parametros de entrada: nenhum
 Retorno: codigo do evento
*************************************************************************/
char* keys;
char inputCode;
void taskObtainEvent(void *pvParameters)
{
    int eventCode;
    int ret;

    for (;;){
        eventCode = NO_EVENTS;

        keys = Keyboard.getKeys();
        inputCode = keys[0];

        /*
        evento                         codigo   implementacao (onde recebe o codigo ou como ativar)
        inicializar e escolher manual: 'm'      keyboard
        inicializar e escolher auto:   'a'      keyboard
        apertar setas para movimentar: 'i'      keyboard
        movimentar os motores:         'r'      evento interno apos executar A03 ou A05
        leitura dos sensores LDR:      'l'      task continua se estiver no modo automatico
        mudar modo com o switch:       's'      keyboard
        salvar posicao atual:          'p'      keyboard
        */

        if ( xQueue != NULL && uxQueueSpacesAvailable(xQueue) > 0) {
            switch (inputCode) {
                case 'm':
                    eventCode = SELECT_MANUAL;
                    ret = xQueueSend( xQueue, (void *)&eventCode, 0 );
                    if( ret != pdTRUE )
                        Keyboard.debug((char*)"Erro ao enviar evento para fila",1);
                    break;
                case 'a':
                    eventCode = SELECT_AUTOMATIC;
                    ret = xQueueSend( xQueue, (void *)&eventCode, 0 );
                    if( ret != pdTRUE )
                        Keyboard.debug((char*)"Erro ao enviar evento para fila",1);
                    break;
                case 'i': // pensar ainda em como fazer a logica de movimentacao para cada lado, talvez usar keys[1]
                    eventCode = INPUT_KEYS;
                    lastMovementKey = keys[1];
                    ret = xQueueSend( xQueue, (void *)&eventCode, 0 );
                    if( ret != pdTRUE )
                        Keyboard.debug((char*)"Erro ao enviar evento para fila",1);
                    break;
                case 'r': // dentro da propia funcao para movimentacao que deve ser checado o modo de operacao
                    eventCode = MOVE_MOTORS;
                    ret = xQueueSend( xQueue, (void *)&eventCode, 0 );
                    if( ret != pdTRUE )
                        Keyboard.debug((char*)"Erro ao enviar evento para fila",1);
                    break;
                case 'l':
                    eventCode = INPUT_SENSORS;
                    ret = xQueueSend( xQueue, (void *)&eventCode, 0 );
                    if( ret != pdTRUE )
                        Keyboard.debug((char*)"Erro ao enviar evento para fila",1);
                    break;
                case 's': // switch alterado -> tratamento deve ser feito dentro de keyboard.cpp msm
                    eventCode = SWITCH_MODE;
                    ret = xQueueSend( xQueue, (void *)&eventCode, 0 );
                    if( ret != pdTRUE )
                        Keyboard.debug((char*)"Erro ao enviar evento para fila",1);
                    break;
                case 'p':
                    eventCode = SAVE_POSITION;
                    ret = xQueueSend( xQueue, (void *)&eventCode, 0 );
                    if( ret != pdTRUE )
                        Keyboard.debug((char*)"Erro ao enviar evento para fila",1);
                    break;
                default:
                    break;
            }
        }

        return;
    }
} // taskObtainEvent


/************************************************************************
 taskStateMachine
 Tarefa para executar a maquina de estados, retira eventos da fila
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void taskStateMachine(void *pvParameters) {
    int eventCode;

    for( ;; ) {
        if( xQueueReceive( xQueue, &eventCode, portMAX_DELAY ) == pdPASS ) {
            if (eventCode != NO_EVENTS)
            {
                actionCode = obtainAction(state, eventCode);
                state = obtainNextState(state, eventCode);
                internEvent = executeAction(actionCode);

                Keyboard.debug((char*)"Estado: ", 0);
                Keyboard.debug(state, 0);
                Keyboard.debug((char*)" Evento: ", 0);
                Keyboard.debug(eventCode, 0);
                Keyboard.debug((char*)" Acao: ", 0);
                Keyboard.debug(actionCode,1);

                // Descomente para imprimir ocupação do stack
                // Serial.print("Task stacks remaining: ");
                // Serial.print(uxTaskGetStackHighWaterMark(xTaskMaqEstados)); Serial.print(" ");
                // Serial.println(uxTaskGetStackHighWaterMark(xTaskObterEvento));
            }
        }
        else {
            Serial.println((char*)"Erro ao receber evento da fila");
        }
    }
} // taskStateMachine

/************************************************************************
 Main
 Setup e Loop principal de controle que executa a maquina de estados
 Parametros de entrada: nenhum
 Retorno: nenhum
*************************************************************************/
void setup() {
    Serial.begin(115200);

    initSystem();
    Serial.println((char*)"Sistema iniciado");

    // configure tasks
    xQueue = xQueueCreate(5, sizeof(int));
    if(xQueue != NULL)
    {
        xTaskCreate(taskStateMachine,"taskMaqEstados", TASK1_INTERVAL, NULL, 2, NULL);
        xTaskCreate(taskObtainEvent,"taskObterEvento", TASK2_INTERVAL, NULL, 1, NULL);
    }
    else
    {
        Keyboard.debug((char*)"Erro na criacao da fila.",1);
    }

} // setup

void loop() {
} // loop