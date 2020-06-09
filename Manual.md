# Automação com Rede *MESH*

## Sumário

1. [Introdução](#introducao)
2. [Objetivos](#objetivos)
3. [Desenvolvimento](#desenvolvimento)  
    1. [*Hardware*](#desenvolvimento/hardware)  
        1. [Rede *MESH*](#desenvolvimento/hardware/mesh)  
        2. [Módulo *Infra-Red*](#desenvolvimento/hardware/ir)
    2. [*Firmware*](#desenvolvimento/firmware)
        1. [Rede *MESH*](#desenvolvimento/firmware/mesh)
            1. [Funcionalidades](#desenvolvimento/firmware/mesh/funcionalidades)
            1. [Eventos & Mensagens](#desenvolvimento/firmware/mesh/json)
        2. [Módulo *Infra-Red*](#desenvolvimento/firmware/ir)
            1. [Funcionalidades](#desenvolvimento/firmware/ir/funcionalidades)

<a name="introducao">

## 1 - Introdução

</a>

<a name="objetivos">

## 2 - Ojetivos

</a>

<a name="desenvolvimento">

## 3 - Desenvolvimento

</a>

<a name="desenvolvimento/hardware">

### 3.1 - *Hardware*

</a>

<a name="desenvolvimento/hardware/mesh">

#### 3.1.1 - Rede *MESH*

</a>

<a name="desenvolvimento/hardware/ir">

#### 3.1.2 - Módulo *Infra-Red*

</a>

<a name="desenvolvimento/firmware">

### 3.2 - *Firmware*

Nesta seção, é apresentado a documentação dos códigos desenvolvidos.

</a>

<a name="desenvolvimento/firmware/mesh">

#### 3.2.1 - Rede *MESH*

A rede desenvolvida foi baseada na biblioteca [painlessMesh](https://gitlab.com/painlessMesh/painlessMesh), que cria e gerencia uma rede local, na qual todos os módulos estão conectados entre si.

Apesar de todos os módulos da rede serem do mesmo tipo, ou seja, sem necessidade de identificar *master* e *slave*, essa biblioteca foi adaptada para definir módulos do tipo ```SERVER```, diferenciados por possuirem conexão *web* e, então, serem ponte entre a rede local e o *backend*.

Os módulos se comunicam por mensagens escritas no formato *JSON*, baseados na biblioteca [ArduinoJson](https://arduinojson.org), que podem ser transmitidas para toda rede ou enviadas especificamente para um módulo, identificado por seu ID (```system_get_chip_id()```).

> NOTA: antes de utilizar a biblioteca, deve-se configurar o *SSID* (MESH_SSID), a senha (MESH_PASSWORD), a porta de comunicação (MESH_PORT) e o intervalo (TASK_INTERVAL) no arquivo ```mesh_network.h```.

</a>

<a name="desenvolvimento/firmware/mesh/funcionalidades">

##### 3.2.1.1 - Funcionalidades

* ```void mesh_network_server_begin(void);```

    Realiza a configuração e inicialização da rede *mesh* em modo ```SERVER```, ou seja:
    
    * Mensagens recebidas são tratadas na função ```void mesh_network_server_received(uint32_t ID, String &msg)```;
    * Novas conexões na rede são tratadas na função ```void mesh_network_new_connection(size_t ID)```;
    * Perca de conexão são tratadas na função ```void mesh_network_dropped_connection(size_t ID)```;
    * Configuração da *Task* ```mesh_network_server_ping_to_client()```, para envios periódicos (TASK_INTERVAL) direcionados a todos os ```CLIENTS```, contendo o ```ID``` do ```SERVER``` e o horário atual (para sincronização do *RTC* interno);
    
* ```void mesh_network_client_begin(void);```

    Realiza a configuração e inicialização da rede *mesh* em modo ```CLIENT```, ou seja:
    
    * Mensagens recebidas são tratadas na função ```void mesh_network_client_received(uint32_t ID, String &msg)```;
    * Novas conexões na rede são tratadas na função ```void mesh_network_new_connection(size_t ID)```;
    * Perca de conexão são tratadas na função ```void mesh_network_dropped_connection(size_t ID)```;
    
* ```void mesh_network_handler(void);```

    Gerenciador da rede *mesh*.
    
    > NOTA: não é recomendado o uso de funções de bloqueio (como *delay()*) no código, visto que poderia ocasionar na perda, total ou parcial, de mensagens.
    
</a>

<a name="desenvolvimento/firmware/mesh/json">

##### 3.2.1.2 - Eventos & Mensagens

* ```Task mesh_network_server_ping_to_client(unsigned long interval, long iterations, Scheduler* scheduler);```

    Rotina do módulo ```SERVER```, implementada para realizar envios periódicos (TASK_INTERVAL) direcionados a todos os módulos ```CLIENTS```:
    
    ```
    {"ID":var1,"subject":var2,"RTC":[RTC[0],RTC[1],RTC[2],RTC[3],RTC[4],RTC[5]]}
    ```
    
    | Variável |  Tipo  |  Tamanho |       Valor       |           Descrição           |
    |:--------:|:------:|:--------:|:-----------------:|:-----------------------------:|
    | var1     | número |  32 bit  | 0x00 ~ 0xFFFFFFFF | Endereço *MAC* do processador |
    | var2     | texto  |     -    |   *server_ping*   | Identificador da operação     |
    | RTC[0]   | número |   8 bit  |       1 ~ 31      | Data: dia                     |
    | RTC[1]   | número |   8 bit  |       1 ~ 12      | Data: mês                     |
    | RTC[2]   | número |  16 bit  |    2013 ~ 9999    | Data: ano                     |
    | RTC[3]   | número |   8 bit  |       0 ~ 23      | Hora: hora                    |
    | RTC[4]   | número |   8 bit  |       0 ~ 59      | Hora: minuto                  |
    | RTC[5]   | número |   8 bit  |       0 ~ 59      | Hora: segundo                 |
    
* ```void mesh_network_client_ping_to_server(uint32_t ID);```

    Rotina dos módulos ```CLIENT```, implementada para responder aos envios periódicos do módulo ```SERVER```:
    
    ```
    {"ID":var1,"subject":var2}
    ```
    
    | Variável |  Tipo  |  Tamanho |       Valor       |           Descrição           |
    |:--------:|:------:|:--------:|:-----------------:|:-----------------------------:|
    | var1     | número |  32 bit  | 0x00 ~ 0xFFFFFFFF | Endereço *MAC* do processador |
    | var2     | texto  |     -    |   *client_ping*   | Identificador da operação     |
    
</a>

<a name="desenvolvimento/firmware/ir">

#### 3.2.2 - Módulo *Infra-Red*

O módulo *IR* foi baseado na biblioteca [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266).

</a>

<a name="desenvolvimento/firmware/ir/funcionalidades">

##### 3.2.2.1 - Funcionalidades

* ```void IR_remote_begin(void);```

    Configura e inicializa o módulo. 
    
* ```void IR_remote_handler(void);```

    Gerenciador do módulo.
    
    Quando o botão do módulo é pressionado, o gerenciador inicia a configuração de um novo controle remoto.

</a>
