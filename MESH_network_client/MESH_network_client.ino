// Descomente para ativar o modo DEBUG
  //#define DEBUG_MODE

  #ifdef DEBUG_MODE
    #define MESH_NETWORK
    #define DATE_TIME_LIB
  #endif

// Cabeçalho da biblioteca
  #include "mesh_network.h"
  #include "date_time.h"

// Setup()
  void setup(void)
  {
    Serial.begin(115200);
    while (!Serial)
      ;

    date_time_begin();

    network_begin();
  }

// Loop()
  void loop(void)
  {
    network_handler();

    date_time_update();
  }
