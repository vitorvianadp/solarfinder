/* Basic Multi Threading Arduino Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
// Please read file README.md in the folder containing this example./*

// #define MAX_LINE_LENGTH (64)

// Define Queue handle
QueueHandle_t QueueHandle;
const int QueueElementSize = 10;
// typedef struct {
//   char line[MAX_LINE_LENGTH];
//   uint8_t line_length;
// } message_t;

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
bool switchAuto = false;
stateTransitionMatrix stateTransition;

/***********************************************************************
 FreeRTOS
 ***********************************************************************/
void taskStateMachine(void *pvParameters);
void taskObtainEvent(void *pvParameters);
// QueueHandle_t xQueue;
// TaskHandle_t xTaskStateMachine, xTaskObtainEvent;
//void TaskWriteToSerial(void *pvParameters);
//void TaskReadFromSerial(void *pvParameters);

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
        //Keyboard.debug((char*)"Modo manual selecionado.",1);
        operationMode = MANUAL;
        break;
    case A02:
        //Keyboard.debug((char*)"Modo automatico selecionado.",1);
        operationMode = AUTOMATIC;
        retval = INPUT_SENSORS;
        break;
    case A03:
        //Keyboard.debug((char*)"Entrada de teclas para posicionar a placa.",1);
        retval = MOVE_MOTORS;
        break;
    case A04:
        // a principio uma acao so, a funcao seria chamada aqui e deveria tratar la dentro como a movimentacao deve ser feita
        //Keyboard.debug((char*)"Movimentacao dos motores.",1);
        //Positioner.moveMotors(Controller.adjustPositions(operationMode, Sensors.getLuminosity(), lastMovementKey));
        Serial.println("Move motor");
        if (operationMode == AUTOMATIC){
          retval = INPUT_SENSORS;
        }
        break;
    case A05:
        //Keyboard.debug((char*)"Leitura de luminosidade dos sensores.",1);
        Display.getInfo(Sensors.getLuminosity());
        Serial.println("leitura sensores");
        retval = MOVE_MOTORS;
        break;
    case A06:
        // funcao deve ser chamada aqui e tratar qual deve ser o novo modo de operacao
        //Keyboard.debug((char*)"Modo de operacao alterado.",1);
        operationMode = 1 - operationMode; // switch
        if (operationMode == AUTOMATIC){
          retval = INPUT_SENSORS;
          switchAuto = false;
        }
        break;
    case A07:
        //Keyboard.debug((char*)"Placa agora esta travada na posicao definida.",1);
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
    BaseType_t xStatus;

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

        if (inputCode == 's'){
          switchAuto = true;
        }

        if (internEvent != NO_EVENTS){
          inputCode = (internEvent == INPUT_SENSORS) ? 'l' : 'r';
        }
        if (switchAuto && inputCode == 'l'){
          inputCode = 's';
        }

        switch (inputCode) {
            case 'm':
                eventCode = SELECT_MANUAL;
                xStatus = xQueueSendToBack( QueueHandle, &eventCode, 0 );
                //if( xStatus != pdPASS )
                    //Keyboard.debug((char*)"Erro ao enviar evento para fila", 1);
                if ( xStatus == pdPASS)
                  Serial.println("manual");
                break;
            case 'a':
                eventCode = SELECT_AUTOMATIC;
                xStatus = xQueueSendToBack( QueueHandle, &eventCode, 0 );
                //if( xStatus != pdPASS )
                    //Keyboard.debug((char*)"Erro ao enviar evento para fila", 1);
                if ( xStatus == pdPASS)
                  Serial.println("automatico");
                break;
            case 'i': // pensar ainda em como fazer a logica de movimentacao para cada lado, talvez usar keys[1]
                eventCode = INPUT_KEYS;
                lastMovementKey = keys[1];
                xStatus = xQueueSendToBack( QueueHandle, &eventCode, 0 );
                //if( xStatus != pdPASS )
                    //Keyboard.debug((char*)"Erro ao enviar evento para fila", 1);
                if ( xStatus == pdPASS)
                  Serial.println("input");
                break;
            case 'r': // dentro da propia funcao para movimentacao que deve ser checado o modo de operacao
                eventCode = MOVE_MOTORS;
                xStatus = xQueueSendToBack( QueueHandle, &eventCode, 0 );
                //if( xStatus != pdPASS )
                    //Keyboard.debug((char*)"Erro ao enviar evento para fila", 1);
                if ( xStatus == pdPASS)
                  Serial.println("motores");
                break;
            case 'l':
                eventCode = INPUT_SENSORS;
                xStatus = xQueueSendToBack( QueueHandle, &eventCode, 0 );
                //if( xStatus != pdPASS )
                    //Keyboard.debug((char*)"Erro ao enviar evento para fila", 1);
                if ( xStatus == pdPASS)
                  Serial.println("sensores");
                break;
            case 's': // switch alterado -> tratamento deve ser feito dentro de keyboard.cpp msm
                eventCode = SWITCH_MODE;
                // if (operationMode == AUTOMATIC){
                //   switchAuto = true;
                // }
                xStatus = xQueueSendToBack( QueueHandle, &eventCode, 0 );
                //if( xStatus != pdPASS )
                    //Keyboard.debug((char*)"Erro ao enviar evento para fila", 1);
                if ( xStatus == pdPASS)
                  Serial.println("switch");
                break;
            case 'p':
                eventCode = SAVE_POSITION;
                xStatus = xQueueSendToBack( QueueHandle, &eventCode, 0 );
                //if( xStatus != pdPASS )
                    //Keyboard.debug((char*)"Erro ao enviar evento para fila", 1);
                if ( xStatus == pdPASS)
                  Serial.println("salva pos");
                break;
            default:
                break;
        }
      vTaskDelay(1);
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
    BaseType_t xStatus;

    for( ;; ) {
        if( xQueueReceive( QueueHandle, &eventCode, portMAX_DELAY ) == pdPASS ) {
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

// The setup function runs once when you press reset or power on the board.
void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  initSystem();
  //Serial.println("Sistema iniciado");

  // Create the queue which will have <QueueElementSize> number of elements, each of size `message_t` and pass the address to <QueueHandle>.
  QueueHandle = xQueueCreate(QueueElementSize, sizeof(int));

  // Check if the queue was successfully created
  if (QueueHandle == NULL) {
    Serial.println("Queue could not be created. Halt.");
    while (1) {
      delay(1000);  // Halt at this point as is not possible to continue
    }
  }

  // Set up two tasks to run independently.
  xTaskCreate(
    taskStateMachine, "TaskStateMachine"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    NULL  // No parameter is used
    ,
    2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,
    NULL  // Task handle is not used here
  );

  xTaskCreate(
    taskObtainEvent, "Task Obtain event", 2048  // Stack size
    ,
    NULL  // No parameter is used
    ,
    1  // Priority
    ,
    NULL  // Task handle is not used here
  );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
  // Serial.printf(
  //   "\nAnything you write will return as echo.\nMaximum line length is %d characters (+ terminating '0').\nAnything longer will be sent as a separate "
  //   "line.\n\n",
  //   MAX_LINE_LENGTH - 1
  // );
}

void loop() {
  // Loop is free to do any other work

  delay(1000);  // While not being used yield the CPU to other tasks
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

// void TaskWriteToSerial(void *pvParameters) {  // This is a task.
//   message_t message;
//   for (;;) {  // A Task shall never return or exit.
//     // One approach would be to poll the function (uxQueueMessagesWaiting(QueueHandle) and call delay if nothing is waiting.
//     // The other approach is to use infinite time to wait defined by constant `portMAX_DELAY`:
//     if (QueueHandle != NULL) {  // Sanity check just to make sure the queue actually exists
//       int ret = xQueueReceive(QueueHandle, &message, portMAX_DELAY);
//       if (ret == pdPASS) {
//         // The message was successfully received - send it back to Serial port and "Echo: "
//         Serial.printf("Echo line of size %d: \"%s\"\n", message.line_length, message.line);
//         // The item is queued by copy, not by reference, so lets free the buffer after use.
//       } else if (ret == pdFALSE) {
//         Serial.println("The `TaskWriteToSerial` was unable to receive data from the Queue");
//       }
//     }  // Sanity check
//   }  // Infinite loop
// }

// void TaskReadFromSerial(void *pvParameters) {  // This is a task.
//   message_t message;
//   for (;;) {
//     // Check if any data are waiting in the Serial buffer
//     message.line_length = Serial.available();
//     if (message.line_length > 0) {
//       // Check if the queue exists AND if there is any free space in the queue
//       if (QueueHandle != NULL && uxQueueSpacesAvailable(QueueHandle) > 0) {
//         int max_length = message.line_length < MAX_LINE_LENGTH ? message.line_length : MAX_LINE_LENGTH - 1;
//         for (int i = 0; i < max_length; ++i) {
//           message.line[i] = Serial.read();
//         }
//         message.line_length = max_length;
//         message.line[message.line_length] = 0;  // Add the terminating nul char

//         // The line needs to be passed as pointer to void.
//         // The last parameter states how many milliseconds should wait (keep trying to send) if is not possible to send right away.
//         // When the wait parameter is 0 it will not wait and if the send is not possible the function will return errQUEUE_FULL
//         int ret = xQueueSend(QueueHandle, (void *)&message, 0);
//         if (ret == pdTRUE) {
//           // The message was successfully sent.
//         } else if (ret == errQUEUE_FULL) {
//           // Since we are checking uxQueueSpacesAvailable this should not occur, however if more than one task should
//           //   write into the same queue it can fill-up between the test and actual send attempt
//           Serial.println("The `TaskReadFromSerial` was unable to send data into the Queue");
//         }  // Queue send check
//       }  // Queue sanity check
//     } else {
//       delay(100);  // Allow other tasks to run when there is nothing to read
//     }  // Serial buffer check
//   }  // Infinite loop
// }