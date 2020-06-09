// Bibliotecas
  #include <Arduino.h>
  #include <painlessMesh.h>
  #include "date_time.h"
  #include "IR_remote.h"

// Cabeçalho da Biblioteca _MESH_NETWORK_H_
#ifndef _MESH_NETWORK_H_
  #define _MESH_NETWORK_H_

  #define MESH_SSID     "EletronNetwork"
  #define MESH_PASSWORD "sGv7XZtTY3ptDLen"
  #define MESH_PORT     5435

  #define TASK_INTERVAL 30000

  #if defined (DEBUG_MODE) && defined (MESH_NETWORK)
    #define MESH_DEBUG
  #endif

  bool network_new_message;
  JsonObject root;

  // Protótipos das funções contidas no arquivo mesh_network
  void network_begin(void);
  void network_handler(void);
  void answer_server(uint32_t ID);
  void network_new_connection_callback(size_t ID);
  void network_dropped_connection_callback(size_t ID);
  void network_received_callback(uint32_t from, String &msg);

#endif /*_MESH_NETWORK_H_*/
