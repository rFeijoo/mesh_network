// Cabeçalho da biblioteca
  #include "date_time.h"

/*  void date_time_begin(void);
    Inicializa o gerenciador do RTC interno.
*/
  void date_time_begin(void)
  {
    date_time_interval_sync = 0;

    date_time_interval_counter = 0;
    
    setSyncProvider(date_time_requestSync);
  }
  
/*  time_t requestSync(void);
    Função de sincronização do RTC.
*/
  time_t date_time_requestSync(void)
  {
    Serial.write(TIME_REQUEST);

    return 0;
  }

/*  void date_time_sync(JsonObject root);
    Sincroniza a data e horário recebidos do módulo master.
*/
  void date_time_sync(JsonObject root)
  {
    if (timeStatus() != timeSet || root["RTC"][4] != minute())
    {
      setTime(root["RTC"][3], root["RTC"][4], root["RTC"][5],
                  root["RTC"][0], root["RTC"][1], root["RTC"][2]);
      
      date_time_interval_sync = 0;
    }
    else
      date_time_interval_sync++;
  }

/*  void date_time_update(void);
    Atualiza as variáveis globais, referentes a data e hora,
    utilizadas nas mensagens enviadas na rede.
*/
  void date_time_handler(void)
  {
    if (millis() > date_time_interval_counter + DATE_TIME_UPDATE_INTERVAL && timeStatus() != timeNotSet)
    {
      date_time = now();
      
      #ifdef DATE_TIME_DEBUG
        Serial.println(String(day(date_time))+"/"+String(month(date_time))+"/"+String(year(date_time)));
        Serial.println(String(hour(date_time))+":"+String(minute(date_time))+":"+String(second(date_time)));
        Serial.println();
      #endif
      
      date_time_interval_counter = millis();
    }
  }
