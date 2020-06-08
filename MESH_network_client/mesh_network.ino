// Cabeçalho da biblioteca
  #include "mesh_network.h"

// Intâncias da biblioteca painlessMesh
  Scheduler userScheduler;
  painlessMesh mesh;

// Variável para armazenamento do ID do módulo central 
  size_t central_id = 0;

/*  void network_begin(bool mode);
    Configura e inicializa o módulo na rede mesh.
*/
  void network_begin(void)
  {
    #ifdef MESH_DEBUG
      mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);
    #endif

    mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
    
    mesh.onReceive(&network_received_callback);
    
    mesh.onNewConnection(&network_new_connection_callback);

    mesh.onDroppedConnection(&network_dropped_connection_callback);
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

    if (root.containsKey("subject"))
    {
      if (String("server_ID").equals(root["subject"].as<String>()))
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

/*  void network_received_callback(uint32_t from, String &msg);
    Envia uma mensagem de confirmação para o módulo master.
*/
  void answer_server(uint32_t ID)
  {
    DynamicJsonDocument jsonBuffer(128);
    
    JsonObject msg = jsonBuffer.to<JsonObject>();

    // ID do módulo 
    msg["ID"] = mesh.getNodeId();

    // Assunto: se identificar como master
    msg["subject"] = "client_ID";

    String str;

    serializeJson(msg, str);

    mesh.sendSingle(ID, str);

    #ifdef MESH_DEBUG
      serializeJson(msg, Serial);
      Serial.printf("\n");
    #endif
  }
