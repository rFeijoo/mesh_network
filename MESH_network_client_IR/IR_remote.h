// Bibliotecas
  #include <Arduino.h>
  #include <ArduinoJson.h>
  #include <IRremoteESP8266.h>
  #include <IRrecv.h>
  #include <IRutils.h>

// Cabeçalho da Biblioteca _IR_REMOTE_H_
#ifndef _IR_REMOTE_H_
  #define _IR_REMOTE_H_

  #define IR_REC_PIN  14

  bool IR_new_message;

  // Protótipos das funções contidas no arquivo date_time
  void IR_remote_begin(void);
  void IR_remote_handler(void);

#endif /*_IR_REMOTE_H_*/
