// Cabeçalho da biblioteca
  #include "IR_remote.h"

// Intâncias da biblioteca IRremoteESP8266
  IRrecv irrecv(IR_REC_PIN);
  decode_results results;

/*  void IR_remote_begin(void);
    Inicializa o módulo IR.
*/
  void IR_remote_begin(void)
  {
    irrecv.enableIRIn();

    IR_new_message = false;
  }

/*  void IR_remote_handler(void);
    Gerencia o módulo IR
*/
  void IR_remote_handler(void)
  {
    if (IR_new_message)
    {
      //editar: master envia nome e quantidade de canais
      //        slave retorna nome do dispositivo (controle sala tv) e suas teclas
      if (String("config_IR").equals(root["subject"].as<String>()))
      {
        // Atualiza ID do master
        central_id = root["ID"];
    
        #ifdef MESH_DEBUG
          Serial.printf("NETWORK: server detected [%u]\n", central_id);
        #endif

        // Atualiza dados do RTC
        date_time_sync(root);

        // Envia uma confirmação para o master
        answer_server(ID);
      }

      IR_new_message = false;
    }
  }
