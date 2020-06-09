// Bibliotecas
  #include <Arduino.h>
  #include <IRremoteESP8266.h>
  #include <IRsend.h>
  #include <IRrecv.h>
  #include <IRutils.h>
  #include <ArduinoJson.h>

// Cabeçalho da Biblioteca _IR_REMOTE_H_
#ifndef _IR_REMOTE_H_
  #define _IR_REMOTE_H_
  
  #define IR_REC_PIN  14  // D5
  #define IR_EMT_PIN  12  // D6
  #define IR_BUT_PIN  13  // D7

  #define IR_UNKW_FREQ 38000 // 38 Khz

  #if defined (DEBUG_MODE) && defined (IR_REMOTE_DBG)
    #define IR_REMOTE_DEBUG
  #endif

  // Protótipos das funções contidas no arquivo IR_remote
  void IR_remote_begin(void);
  void IR_remote_handler(void);
  void IR_remote_register(void);
  bool IR_remote_test_code(void);
  bool IR_remote_save_code(void);
  
#endif /*_IR_REMOTE_H_*/
