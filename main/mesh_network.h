// Bibliotecas
  #include <Arduino.h>
  #include <painlessMesh.h>
  #include "date_time.h"

// Cabeçalho da Biblioteca _MESH_NETWORK_H_
#ifndef _MESH_NETWORK_H_
  #define _MESH_NETWORK_H_

  #define MESH_SSID     "EletronNetwork"
  #define MESH_PASSWORD "sGv7XZtTY3ptDLen"
  #define MESH_PORT     5435

  #define TASK_INTERVAL 30

  #if defined (DEBUG_MODE) && defined (NETWORK__DBG)
    #define MESH_DEBUG
  #endif

  // Protótipos das funções contidas no arquivo mesh_network
  void mesh_network_handler(void);
  void mesh_network_server_begin(void);
  void mesh_network_client_begin(void);
  void mesh_network_new_connection(size_t ID);
  void mesh_network_dropped_connection(size_t ID);
  void mesh_network_client_ping_to_server(uint32_t ID);
  void mesh_network_server_received(uint32_t ID, String &msg);
  void mesh_network_client_received(uint32_t ID, String &msg);

#endif /*_MESH_NETWORK_H_*/
