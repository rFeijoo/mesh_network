// Cabeçalho da biblioteca
  #include "mesh_network.h"

// Intâncias da biblioteca painlessMesh
  Scheduler userScheduler;
  painlessMesh mesh;

/*  Task send_server_ID(TASK_INTERVAL, TASK_FOREVER, [](){});
    Envia dados na rede, a cada  TASK_INTERVAL ms, para configuração de 
    novos dispositivos recém conectados, além de atualizar o RTC dos
    módulos.
*/
  Task send_server_ID(TASK_INTERVAL, TASK_FOREVER, []()
  {
    DynamicJsonDocument jsonBuffer(256);
    
    JsonObject msg = jsonBuffer.to<JsonObject>();
    
    msg["ID"] = mesh.getNodeId();
    
    msg["subject"] = "server_ID";
    
    msg["RTC"][0]  = 8;
    msg["RTC"][1]  = 6;
    msg["RTC"][2]  = 2020;
    msg["RTC"][3]  = 0;
    msg["RTC"][4]  = 10;
    msg["RTC"][5]  = 15;

    String str;

    serializeJson(msg, str);
    
    mesh.sendBroadcast(str);

    #ifdef MESH_DEBUG
      serializeJson(msg, Serial);
      Serial.printf("\n");
    #endif
  });

/*  void network_begin(bool mode);
    Configura e inicializa o módulo na rede mesh.
*/
  void network_begin(void)
  {
    #ifdef MESH_DEBUG
      mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);
    #endif

    mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
    
    mesh.onReceive(&network_received_callback);
    
    mesh.onNewConnection(&network_new_connection_callback);

    mesh.onDroppedConnection(&network_dropped_connection_callback);
    
    userScheduler.addTask(send_server_ID);
    send_server_ID.enable();
  }

/*  void network_received_callback(uint32_t ID, String &msg);
    Qualquer mensagem recebida por este módulo dispara a chamada desta função.
*/
  void network_received_callback(uint32_t ID, String &msg)
  {
    Serial.printf("--> \"%u\" says:\n", ID);
    Serial.printf("%s\n\n", msg.c_str());

    DynamicJsonDocument jsonBuffer(1024 + msg.length());
    
    DeserializationError error = deserializeJson(jsonBuffer, msg);
    
    if (error)
      return;
      
    JsonObject root = jsonBuffer.as<JsonObject>();

    if (String("client_ID").equals(root["subject"].as<String>()))
    {
      Serial.printf("MESSAGE FROM CLIENT\n");
    }
  }

/*  void network_new_connection_callback(size_t ID);
    Novas conexões na rede disparam a chamada desta função.
*/
  void network_new_connection_callback(size_t ID)
  {
    Serial.printf("--> New connection:\n\tID: \"%u\"\n", ID);
  }

/*  void network_dropped_connection_callback(size_t ID);
    Novas conexões na rede disparam a chamada desta função.
*/
  void network_dropped_connection_callback(size_t ID)
  {
    Serial.printf("--> Dropped connection:\n\tID: \"%u\"\n", ID);
  }

/*  void network_handler(void);
    Gerencia a rede mesh.
*/
  void network_handler(void)
  {
    mesh.update();
  }
