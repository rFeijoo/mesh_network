// Descomente para ativar o modo DEBUG
  //#define DEBUG_MODE

  #ifdef DEBUG_MODE
    #define NETWORK__DBG
    #define DATE_TIME_DBG
    #define IR_REMOTE_DBG
  #endif

// Cabeçalho da biblioteca
  #include "mesh_network.h"
  #include "date_time.h"

// Setup()
  void setup(void)
  {
    // Inicializa o Serial Console
    Serial.begin(115200);
    while (!Serial)
      ;

    // Inicializa o módulo IR
    IR_remote_begin();

    // Inicializa o gerenciador do RTC
//    date_time_begin();

    // Inicializa a rede MESH
//    mesh_network_server_begin(); // modo SERVER
//    mesh_network_client_begin(); // modo CLIENT
  }

// Loop()
  void loop(void)
  {
    // Atualiza a rede MESH
//    mesh_network_handler();

    // Atualiza o RTC
//    date_time_handler();

    // Atualiza o módulo IR
    IR_remote_handler();
  }
