// Bibliotecas
  #include <Arduino.h>
  #include <TimeLib.h>
  #include <ArduinoJson.h>

// Cabeçalho da Biblioteca _DATE_TIME_H_
#ifndef _DATE_TIME_H_
  #define _DATE_TIME_H_

  #define TIME_HEADER  "T"
  #define TIME_REQUEST  7

  #define DATE_TIME_UPDATE_INTERVAL 3000

  #if defined (DEBUG_MODE) && defined (DATE_TIME_LIB)
    #define DATE_TIME_DEBUG
  #endif

  // Protótipos das funções contidas no arquivo date_time
  void date_time_begin(void);
  void date_time_handler(void);
  time_t date_time_requestSync(void);
  void date_time_sync(JsonObject root);

  uint8_t date_time_interval_sync;

  uint32_t date_time_interval_counter;

  time_t date_time;
  
#endif /*_DATE_TIME_H_*/
