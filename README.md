# Comunicação Serial com RP2040

Projeto criado para consolidar os conceitos aprendidos sobre o uso de interfaces de comunicação serial no RP2040. Além disso, o projeto também visa explorar as funcionalidades da placa de desenvolvimento BitDogLab.

## Objetivos
- Compreender o funcionamento e aplicação de comunicação serial em microcontroladores.
- Aplicar conhecimentos sobre UART e I2C.
- Monitorar e controlar LEDs comuns e LEDs endereçáveis WS2812.
- Fixar o estudo do uso de botões de acionamento, interrupções e Debounce.
- Desenvolver um projeto funcional que combine hardware e software.

## Descrição do Projeto
O projeto utiliza os seguintes componentes conectados à placa BitDogLab:
- **Matriz 5x5 de LEDs (endereçáveis) WS2812**, conectada à GPIO 7.
- **LED RGB**, com os pinos conectados às GPIOs 11 e 12.
- **Botão A** conectado à GPIO 5.
- **Botão B** conectado à GPIO 6.
- **Display SSD1306** conectado via I2C (GPIO 14 e GPIO 15).

## Funcionalidades do Projeto
1. **Entradas de caracteres via PC**:
   - Caracteres digitados no monitor serial são exibidos no **display SSD1306**.
   - Quando um número entre 0 e 9 é digitado, além de ser exibido no display, o símbolo correspondente a ele é exibido também na **matriz WS2812**.

2. **Interação com os botões**:
   - Ao pressionar o botão A, o estado do LED Verde é alterado.
   - Ao pressionar o botão B, o estado do LED Azul é alterado.
   - As mudanças de estado dos LEDs são registradas através de uma mensagem no display SSD1306 e também através de um texto descritivo enviado ao monitor serial.

## Requisitos do Projeto
- [X] Uso de interrupções nas funcionalidades relacionadas aos botões.
- [X] Implementação do tratamento de bouncing dos botões via software.
- [X] Uso de LEDs comuns e LEDs WS2812.
- [X] Utilização do Display SSD1306.
- [X] Envio de informações pela UART.
- [X] Organização do código.

## Como Compilar e Executar
1. **Pré-requisitos**:
   - Ter o ambiente de desenvolvimento para o Raspberry Pi Pico configurado (compilador, SDK, etc.).
   - CMake instalado.

2. **Compilação**:
   - Clone o repositório ou baixe os arquivos do projeto.
   - Navegue até a pasta do projeto e crie uma pasta de build:
     ```bash
     mkdir build
     cd build
     ```
   - Execute o CMake para configurar o projeto:
     ```bash
     cmake ..
     ```
   - Compile o projeto:
     ```bash
     make
     ```

3. **Upload para a placa**:
   - Conecte o Raspberry Pi Pico ao computador.
   - Copie o arquivo `.uf2` gerado para a placa.

## Simulação no Wokwi
Para visualizar a simulação do projeto no Wokwi:
1. Instale e configure o simulador Wokwi seguindo as instruções encontradas no link a seguir:  
   [Introdução ao Wokwi para VS Code](https://docs.wokwi.com/pt-BR/vscode/getting-started).
2. Abra o arquivo `diagram.json` no VS Code.
3. Clique em "Start Simulation".

## Vídeo de Demonstração
O vídeo demonstrativo do projeto pode ser encontrado no link abaixo:  
[Link para o vídeo](https://youtu.be/bPbepU82pqQ)