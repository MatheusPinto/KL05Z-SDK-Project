[![en](https://img.shields.io/badge/lang-en-red.svg)](/README.md)


# Projeto com SDK para o KL05Z

Projeto criado no Kinetis Design Studio(KDS)/Eclipse da NXP para auxílio no desenvolvimento de aplicações com microcontroladores, em especial para o Kinetis KL05Z.
Essa SDK (Software Development Kit) foi pensada para ser utilizada em qualquer família de microcontroladores, de qualquer fabricante, bastando apenas modificações pontuais em alguns arquivos.


## Diretórios

O projeto possui uma estrutura pré-definida para um uso padronizado e facilitado da SDK.

O SDK está estruturado dentro da pasta `Sources`. Os diretórios acima deste são específicos para a configuração do microcontrolador no KDS.

* `mcu`: Possui a API e as implementações específicas do microcontrolador utilizado.

  * `drivers`: Implementação dos drivers de periféricos do microcontrolador.

  * `common.h`: Arquivo com definições comuns aos drivers.

* `libraries`: Bibliotecas genéricas para auxílio no desenvolvimento.

* `system`: Módulo com os arquivos relacionados a configurações do sistema. Destes vale mencionar alguns.

  * `os`: Contém uma API genérica de Sistema Operacional (SO). Dessa forma, é recomendado portar as funções do SO especifico (ex.: FreeRTOS, Zephyr, TinyOS, etc.) para dentro dessa camada de software. 

  * `system.h`: Arquivo de cabeçalho com a definição da função `System_Init`, que deve ser chamada no ínicio da função `main`.

  * `hardware_init.h/.c`: Deve conter a implementação, fornecida pelo usuário, do código de inicialicação dos hardware do sistema (periféricos internos/externos e sistema de clocks). A inicialização deve ser disparada pela função `System_Hardware_Init` que será invocada dentro de `System_Init`.

  * `software_init.h/.c`: Deve conter a implementação, fornecida pelo usuário, do código de inicialicação das bibliotecas de software do sistema (ex.: Delay, geração de semente para números aleatórios, etc.). A inicialização deve ser disparada pela função `System_Software_Init` que será invocada dentro de `System_Init`.

* `app_tasks`: Implementação das tarefas de aplicação. É interessante que cada tarefa seja implementada como uma única função em C, com protótipo disponível em arquivo de cabeçalho.

* `events`: Funções que são chamadas devido a um evento interno ou externo ao microcontrolador.

  * `isr`: Contém as funções de tratamento de interrupção (ISR) do microcontrolador.

  * `os`: Contém as funções de tratamento a eventos do sistema operacional utilizado.

* `fsm`: Caso o firmware seja baremetal (ou seja, sem SO), recomenda-se desenvolvimento com modelo de máquina de estados (FSM). Aqui encontra-se um modelo de FSM para implementação do sistema.

* `examples`: Contém códigos de exemplo com a função `main`. Dessa forma, deve-se sempre desabilitar da compilação os outros arquivos desse diretório para não ocorrer erro de múltiplas definições do `main`.

## Convenção de codificação
No código não desenvolvido por terceiros, existe uma convenção de codificação definida.

#### Funções
Todas as funções começam com o nome sugestivo do módulo ao qual as funções pertencem, começando por letra maiúscula. Esse nome é seguido por sublinhado `_` e o nome específico da função começando pela letra maiúscula.

```c
  <NomeDoMódulo>_NomeDaFunção();
```

Exemplos:

```c
  Delay_WaitMS();
  GPIO_SetPin();
  Util_ClearMask();
  LCD_Print();
```

Para módulos complexos, possuindo diversos sub-módulos (exemplo: SO), é permitido usar a seguinte convenção:

 ```c
  <NomeDoMódulo>_<NomeDoSubMódulo>_NomeDaFunção();
```

Exemplos:

```c
  OS_Mutex_Take();
  OS_Task_Create();
  OS_Heap_Alloc();
  OS_Sched_Start();
```

#### Variáveis
Todas as variáveis iniciam com letra mínuscula. Se formadas por mais de uma palavra, todas as palavras seguintes começam com letra maiúscula.

Exemplos:

```c
  myFlag;
  x;
  dataIn;
  lastWokenTime;
```

* Variáveis com escopo local (ou seja, declaradas dentro de funções) devem ser nomeadas conforme descrito acima.

* Variáveis globais: sejam com escopo estático (static) ou externo (extern) devem ter seu nome precedido de `g_`.

* Variáveis constantes: devem ter seu nome precedido de sublinhado `_`.

#### Tipos definidos
Tipos definidos com o uso de `typedef` seguem a mesma conveção de nome de variáveis, porém terminados com `_t`. Também, se aconselha a colocar como primeira palavra, o nome do módulo onde o tipo foi definido. 

Exemplos:

```c
  myType_t;
  uartParity_t;
  adcStatusFlag_t;
  tpmPrescalerValues_t;
```


#### Macros e valores de enumeração
Todas as letras em maiúsculo, com palavras separadas por sublinhado `_`. Também, se aconselha a colocar como primeira palavra, o nome do módulo onde o tipo foi definido.

Exemplos:

```c
  #define MY_DEFINITION
  #define BUFFER_MAX_LENGTH

  typedef enum{
    UART_ONE_STOP_BIT = 0U, 
    UART_TWO_STOP_BIT 
  }uartStopBitNum_t;
```
