// Cabeçalho da biblioteca
  #include "mesh_network.h"

// Setup()
  void setup(void)
  {
    Serial.begin(115200);
    while (!Serial)
      ;

    network_begin();
  }

// Loop()
  void loop(void)
  {
    network_handler();
  }
