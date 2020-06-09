// Cabeçalho da biblioteca
  #include "mesh_network.h"

// Intâncias da biblioteca painlessMesh
  Scheduler userScheduler;
  painlessMesh mesh;

// Variável para armazenamento do ID do módulo central 
  size_t central_id = 0;

/*  Task mesh_network_server_ping_to_client(TASK_INTERVAL, TASK_FOREVER, [](){});
    Envia dados na rede, a cada  TASK_INTERVAL ms, para configuração de 
    novos dispositivos recém conectados, além de atualizar o RTC dos
    módulos.
*/
  Task mesh_network_server_ping_to_client(TASK_INTERVAL * TASK_SECOND, TASK_FOREVER, []()
  {
    DynamicJsonDocument jsonBuffer(256);
    
    JsonObject msg = jsonBuffer.to<JsonObject>();
    
    msg["ID"] = mesh.getNodeId();
    
    msg["subject"] = "server_ping";
    
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

/*  void mesh_network_server_begin(void);
    Configura e inicializa o módulo, em modo SERVER, na rede.
*/
  void mesh_network_server_begin(void)
  {
    #ifdef MESH_DEBUG
      mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);
    #endif
    
    mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);

    mesh.onReceive(&mesh_network_server_received);
    
    mesh.onNewConnection(&mesh_network_new_connection);

    mesh.onDroppedConnection(&mesh_network_dropped_connection);

    userScheduler.addTask(mesh_network_server_ping_to_client);
    
    mesh_network_server_ping_to_client.enable();
  }

/*  void mesh_network_client_begin(void);
    Configura e inicializa o módulo, em modo CLIENT, na rede.
*/
  void mesh_network_client_begin(void)
  {
    #ifdef MESH_DEBUG
      mesh.setDebugMsgTypes(ERROR | DEBUG | CONNECTION);
    #endif

    mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);

    mesh.onReceive(&mesh_network_client_received);
    
    mesh.onNewConnection(&mesh_network_new_connection);

    mesh.onDroppedConnection(&mesh_network_dropped_connection);
  }

/*  void mesh_network_handler(void);
    Gerenciador da rede.
*/
  void mesh_network_handler(void)
  {
    mesh.update();

    yield();
  }

/*  void mesh_network_client_ping_to_server(uint32_t ID);
    Envia uma mensagem de confirmação para o módulo SERVER.
*/
  void mesh_network_client_ping_to_server(uint32_t ID)
  {
    DynamicJsonDocument jsonBuffer(128);
    
    JsonObject msg = jsonBuffer.to<JsonObject>();

    // ID do módulo 
    msg["ID"] = mesh.getNodeId();

    // Assunto: se identificar como master
    msg["subject"] = "client_ping";

    String str;

    serializeJson(msg, str);

    mesh.sendSingle(ID, str);

    #ifdef MESH_DEBUG
      serializeJson(msg, Serial);
      Serial.printf("\n");
    #endif
  }

/*  void mesh_network_new_connection(size_t ID);
    Novas conexões na rede disparam a chamada desta função.
*/
  void mesh_network_new_connection(size_t ID)
  {
    Serial.printf("--> New connection:\n\tID: \"%u\"\n", ID);
  }

/*  void mesh_network_dropped_connection(size_t ID);
    Perca de conexões na rede disparam a chamada desta função.
*/
  void mesh_network_dropped_connection(size_t ID)
  {
    Serial.printf("--> Dropped connection:\n\tID: \"%u\"\n", ID);
  }

/*  void mesh_network_server_received(uint32_t ID, String &msg);
    Caso o módulo seja do tipo SERVER, qualquer mensagem recebida
    dispara a chamada desta função.
*/
  void mesh_network_server_received(uint32_t ID, String &msg)
  {
    Serial.printf("--> \"%u\" says:\n", ID);
    Serial.printf("%s\n\n", msg.c_str());

    DynamicJsonDocument jsonBuffer(1024 + msg.length());
    
    DeserializationError error = deserializeJson(jsonBuffer, msg);
    
    if (error)
      return;
      
    JsonObject root = jsonBuffer.as<JsonObject>();

    if (String("client_ping").equals(root["subject"].as<String>()))
    {
      Serial.printf("MESSAGE FROM CLIENT\n");
    }
  }

/*  void mesh_network_client_received(uint32_t ID, String &msg);
    Caso o módulo não seja do tipo SERVER, qualquer mensagem recebida
    dispara a chamada desta função.
*/
  void mesh_network_client_received(uint32_t ID, String &msg)
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
      if (String("server_ping").equals(root["subject"].as<String>()))
      {
        // Atualiza ID do master
        central_id = root["ID"];
    
        #ifdef MESH_DEBUG
          Serial.printf("NETWORK: server detected [%u]\n", central_id);
        #endif

        // Atualiza dados do RTC
        date_time_sync(root);

        // Envia uma confirmação para o server
        mesh_network_client_ping_to_server(ID);
      }
    }
  }
