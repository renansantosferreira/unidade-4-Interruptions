# Projeto: Matriz WS2812 com Controle por Botões

Este projeto foi desenvolvido para controlar uma matriz de LEDs WS2812 utilizando o Raspberry Pi Pico (ou qualquer outra placa compatível) e botões para navegar entre números de 0 a 9, exibidos em uma matriz 5x5. A interface de controle é feita através de dois botões, e o LED RGB pisca em um intervalo regular.

## Funcionalidades

- **Matriz de LEDs WS2812**: Exibe números de 0 a 9 em uma matriz de 5x5 LEDs WS2812.
- **Controle por Botões**: 
  - O botão **A** incrementa o número exibido.
  - O botão **B** decrementa o número exibido.
- **LED RGB**: Pisca o LED vermelho a cada 100 ms como um indicativo visual.

## Componentes Necessários

- 1 Raspberry Pi Pico (ou similar)
- 1 Matriz de LEDs WS2812 (5x5 LEDs)
- 2 Botões para incrementar e decrementar os números exibidos
- LED RGB (opcional)

## Diagrama de Conexão

### Conexões

- **LED RGB**:
  - LED Verde (pino 11)
  - LED Azul (pino 12)
  - LED Vermelho (pino 13)
- **Botões**:
  - Botão A (pino 5)
  - Botão B (pino 6)
- **Matriz WS2812**:
  - Data (pino 7)

## Configuração

### Pinos de Controle

- **LED Verde**: Pino 11
- **LED Azul**: Pino 12
- **LED Vermelho**: Pino 13
- **Botão A**: Pino 5
- **Botão B**: Pino 6
- **Matriz WS2812**: Pino 7

### Parâmetros de Configuração

- **INTENSIDADE**: Define a intensidade dos LEDs (valor de 0 a 255).
- **MATRIX_SIZE**: Define o tamanho da matriz 5x5 (total de 25 LEDs WS2812).

## Como Funciona

1. **Inicialização**: O programa configura os pinos do Raspberry Pi Pico e inicializa a matriz WS2812 utilizando o PIO (Programmable I/O).
2. **Botões**: Ao pressionar o botão **A**, o número exibido na matriz aumenta de 1, e ao pressionar o botão **B**, o número diminui de 1. A contagem é circular, ou seja, ao alcançar o número 9, o próximo número exibido será 0 e vice-versa.
3. **Piscar LED**: O LED vermelho pisca continuamente a cada 100 ms, alterando seu estado (aceso/desligado) para indicar atividade.
4. **Matriz 5x5**: A matriz de LEDs exibe os números de 0 a 9, representados em um formato gráfico em 5x5 LEDs. Cada número é mapeado para uma matriz de 25 LEDs que são acesos de acordo com a representação gráfica do número.

## Estrutura de Diretórios

```text
project/
│
├── src/                # Código-fonte do projeto
│   ├── unidade-4-Interruptions.c          # Arquivo principal com a lógica de controle
│   
│   └── ws2812.h        # Cabeçalho do código PIO para controle dos LEDs
│
├── README.md           # Este arquivo
└── Makefile            # Makefile para compilação do código
