// Cabeçalho da biblioteca
  #include "IR_remote.h"

// Intâncias da biblioteca IRremoteESP8266
  IRrecv irrecv(IR_REC_PIN, 1024, 50, false);
  IRsend irsend(IR_EMT_PIN);
  decode_results results;

// Variável global para armazenamento dos códigos IR
  uint64_t ir_codes[3] = {0};
  uint8_t  ir_count = 0;

/*  void IR_remote_begin(void);
    Inicializa o módulo IR.
*/
  void IR_remote_begin(void)
  {
    pinMode(IR_BUT_PIN, INPUT);

    irsend.begin();
  }

/*  void IR_remote_handler(void);
    Gerenciador do módulo IR.
*/
  void IR_remote_handler(void)
  {
    // Rotina de configuração de novo controle
    if (digitalRead(IR_BUT_PIN))
      IR_remote_register();

    // Adicionar rotina para verificação de JSON recebido
    // buscar no JSON qual código o irsender deve transmitir
  }

/*  void IR_remote_register(void);
    Inicializa o processo de cadastro de um controle remote,
    caso o botão seja pressionado.
*/
  void IR_remote_register(void)
  {
    delay(250);

    long timer = millis();
    bool done = false;

    irrecv.enableIRIn();
  
    Serial.println("Waiting for IR...");
  
    while (!done && (millis() < timer + 5000))
    {
      if (irrecv.decode(&results))
      {
        serialPrintUint64(results.value, HEX);
        Serial.println();
        
        IR_remote_test_code();
        
        irrecv.resume();
      }
      
      yield();
    }

    irrecv.disableIRIn();
  
    delay(250);
  }

/*  bool IR_remote_test_code(void);
    Após receber um código IR, testa o mesmo 5 vezes.
    Caso o botão seja pressionado durante esse período, a rotina
    entende que o código é funcional.
*/
  bool IR_remote_test_code(void)
  {
    long timer;
    bool done = false;
  
    if (results.value == 0x00 || results.value >= 0xFFFFFF)
      return false;
  
    decode_type_t protocol = results.decode_type;
      
    uint16_t size = results.bits;
      
    bool success = true;
  
    for (int i=0; i<5; i++)
    {
      Serial.println("["+String(i)+"]: Testing IR... Press button if the code worked!");
      
      if (protocol == decode_type_t::UNKNOWN)
      {
        uint16_t *raw_array = resultToRawArray(&results);
        
        size = getCorrectedRawLength(&results);
        
        irsend.sendRaw(raw_array, size, IR_UNKW_FREQ);
        
        delete [] raw_array;
      }
      else if (hasACState(protocol))
        success = irsend.send(protocol, results.state, size / 8);
      else
        success = irsend.send(protocol, results.value, size);
  
      timer = millis();
      while (millis() < timer + 2000)
      {
        if (digitalRead(IR_BUT_PIN))
          return IR_remote_save_code();
          
        yield();
      }
    }
  
    Serial.println("IR NOT saved as a valid command.");
  
    return false;
  }
  
/*  bool IR_remote_save_code(void);
    Armazena o código IR em um buffer, para posteriormente
    passar todos os códigos, de uma só vez, via JSON, para o
    servidor.
*/
  bool IR_remote_save_code(void)
  {
    ir_codes[ir_count] = results.value;

    ir_count = (ir_count + 1) % 3;
    
    Serial.println("IR saved as a valid command.");
    
    for (int k=0; k<3; k++)
    {
      serialPrintUint64(ir_codes[k], HEX);
      Serial.println();
    }
    
    return true;
  }
