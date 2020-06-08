// Bibliotecas
  #include <Arduino.h>
  #include <painlessMesh.h>

// Cabeçalho da Biblioteca _MESH_NETWORK_H_
#ifndef _MESH_NETWORK_H_
  #define _MESH_NETWORK_H_

  #define MESH_SSID     "EletronNetwork"
  #define MESH_PASSWORD "sGv7XZtTY3ptDLen"
  #define MESH_PORT     5435

  #define TASK_INTERVAL 30000

  // Descomente para ativar o modo DEBUG
  // #define MESH_DEBUG

  // Protótipos das funções contidas no arquivo mesh_network
  void network_begin(void);
  void network_handler(void);
  void network_received_callback(uint32_t from, String &msg);
  void network_new_connection_callback(size_t ID);
  void network_dropped_connection_callback(size_t ID);

#endif /*_MESH_NETWORK_H_*/
