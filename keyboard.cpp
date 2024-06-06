#include <Arduino.h>
#include <Wifi.h>
#include "config.h"
#include "keyboard.h"

KeyboardClass Keyboard;
NetworkServer server(80);

/************************************************************************
 construtor
*************************************************************************/
KeyboardClass::KeyboardClass()
{
}

void KeyboardClass::init() {
  server.begin();
}

/************************
 debug
 Funcao usada durante o desenvolvimento do codigo para testes com serial
 entradas
   texto : texto a ser enviado no serial
 saidas
   nenhuma
*************************/
void KeyboardClass::debug(char *texto, int ln)
{
    #ifndef DEBUG
        if (ln == 0)
            Serial.print(texto);
        else
            Serial.println(texto);
    #endif
}

//override
void KeyboardClass::debug(int value, int ln)
{
    #ifndef DEBUG
        if (ln == 0)
            Serial.print(value);
        else
            Serial.println(value);
    #endif
}

/************************
 getKeys
 Obtem tecla de movimentacao no caso manual
 Obtem status do switch para determinar o modo
 entradas
   nenhuma
 saidas
   informacao recebida na interface ou NO_INFO
*************************/
char* KeyboardClass::getKeys()
{
    // nessa funcao pode ser feita talvez uma conversao do evento de apertar o switch ou apertar tecla de movimentacao
    // para o codigo correspondente na logica de movimentacao (meio que a logica de tratamento fica aqui e de movimentacao nao fica aqui)

    /*
    evento:                        codigo   variacoes
    inicializar e escolher manual: 'm'
    inicializar e escolher auto:   'a'
    apertar setas para movimentar: 'i'      'iu' -> up;   'il' -> left;
                                            'id' -> down; 'ir' -> right
    mudar modo com o switch:       's'
    salvar posicao atual:          'p'
    */

/*
    */
    int read_count = 0;
    

    // if (Serial.available() > 0)
    // {
    //     read_count = Serial.readBytesUntil('\n', buf, sizeof(buf) / sizeof(buf[0]) - 1);
    // }

    // buf[read_count] = '\0';
    // if (read_count > 0)
    // {
    //     Serial.println(buf);
    // }
    
      NetworkClient client = server.accept();  // listen for incoming clients

    if (client) {                     // if you get a client,
      Serial.println("New Client.");  // print a message out the serial port
      String currentLine = "";        // make a String to hold incoming data from the client
      while (client.connected()) {    // loop while the client's connected
        if (client.available()) {     // if there's bytes to read from the client,
          char c = client.read();     // read a byte, then
          Serial.write(c);            // print it out the serial monitor
          if (c == '\n') {            // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
              //client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
              //client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else {  // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }

          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /m")) {

            buf[read_count++] = 'm'; // GET /H turns the LED on
          }
          if (currentLine.endsWith("GET /a")) {
            buf[read_count++] = 'a';  // GET /L turns the LED off
          }
         
          if (currentLine.endsWith("GET /r")) {
            buf[read_count++] = 'r';  // GET /L turns the LED off
          }
          if (currentLine.endsWith("GET /l")) {
            buf[read_count++] = 'l';  // GET /L turns the LED off
          }
          if (currentLine.endsWith("GET /s")) {
            buf[read_count++] = 's';  // GET /L turns the LED off
          }
          if (currentLine.endsWith("GET /p")) {
            buf[read_count++] = 'p';  // GET /L turns the LED off
          }
           if (currentLine.endsWith("GET /iu")) {
            buf[read_count++] = 'i';
            buf[read_count++] = 'u'; // GET /L turns the LED off
          }
           if (currentLine.endsWith("GET /id")) {
            buf[read_count++] = 'i';
            buf[read_count++] = 'd'; // GET /L turns the LED off
          }
           if (currentLine.endsWith("GET /il")) {
            buf[read_count++] = 'i';
            buf[read_count++] = 'l'; // GET /L turns the LED off
          }
           if (currentLine.endsWith("GET /ir")) {
            buf[read_count++] = 'i';
            buf[read_count++] = 'r'; // GET /L turns the LED off
          }
        }
      }
      // close the connection:
      client.stop();
      Serial.println("Client Disconnected.");
    }

      //if (Serial.available() > 0)
      //{
        //  read_count = Serial.readBytesUntil('\n', buf, sizeof(buf) / sizeof(buf[0]) - 1);
      //}
      buf[read_count] = '\0';
    return buf;
}