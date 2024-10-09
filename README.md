# Cubetto 🤖

## Montagem do Arduino

![image](https://github.com/user-attachments/assets/9a615668-cbff-4605-bbc0-f5ea874ace50)

![image](https://github.com/user-attachments/assets/6056decb-53ad-4f05-86f7-192a844a66d3)

## Softwares Necessários
  - [Arduino IDE](https://www.arduino.cc/en/software)
  - Serial Bluetooth Terminal **Utilizar no celular, baixe na PlayStore**

## Configurações do Arduino IDE
- Selecione o Arduino e Porta
![image](https://github.com/user-attachments/assets/a384b525-e998-411a-8187-69109b4ef4ec)

- Coloque essas configurações no monitor serial.
![image](https://github.com/user-attachments/assets/79371662-345a-4caa-8e9c-a80d34b732cb)

---

## Entrar no Modo AT (para HC-05)
Para o módulo HC-05, siga estes passos para entrar no modo AT:
  1. Desconecte o pino VCC do HC-05 (desligue o módulo).
  2. Pressione e segure o botão no módulo Bluetooth (se o seu modelo tiver um botão).
  3. Conecte o pino VCC novamente enquanto ainda estiver pressionando o botão.
  4. A luz do módulo Bluetooth começará a piscar lentamente (1 vez a cada 2 segundos), indicando que está no modo AT.
  5. Solte o botão. Agora o módulo deve estar pronto para receber comandos AT.

---

## Código para Comunicação Serial com Bluetooth

```cpp
#include "SoftwareSerial.h"

// Pinos TX e RX para comunicação com o Bluetooth
SoftwareSerial bluetooth(10, 11); // TX, RX (Bluetooth)

void setup() {
  // Inicializa a comunicação com o PC via serial
  Serial.begin(38400); 
  // Inicializa a comunicação com o Bluetooth (AT mode)
  bluetooth.begin(38400); // Configura para 9600 bps para comandos AT
}

void loop() {
  // Recebe dados do Monitor Serial e envia ao Bluetooth
  if (Serial.available()) {
    char r = Serial.read();
    bluetooth.write(r);  // Envia ao HC-05
    Serial.print(r);  // Echo
  }
  // Recebe dados do Bluetooth e envia ao Monitor Serial
  if (bluetooth.available()) {
    char r = bluetooth.read();
    Serial.print(r);  // Exibe a resposta do HC-05
  }
}
```
---

## Comandos AT para Configuração do Módulo Bluetooth HC-05

1. **Verificar Conexão**
    - Comando: `AT`
    - Resposta esperada: `OK`
   
2. **Verificar e Configurar o Baud Rate**
    - **Verificar Baud Rate Atual**  
      Comando: `AT+UART?`  
      Resposta esperada: `+UART:<baud>,<stop_bits>,<parity>`  
    - **Configurar Baud Rate para 38400**  
      Comando: `AT+UART=38400,0,0`  
      Resposta esperada: `OK`

3. **Configurar Modo de Operação**
    - **Módulo no Modo Escravo**  
      Comando: `AT+ROLE=0`  
      Resposta esperada: `OK`
    - **Módulo no Modo Mestre**  
      Comando: `AT+ROLE=1`  
      Resposta esperada: `OK`

4. **Configuração de Modo de Conexão (CMODE)**
    - **Verificar o Valor Atual**  
      Comando: `AT+CMODE`  
      Resposta esperada: `+CMODE:1`
    - **Permitir Conexão com Qualquer Dispositivo**  
      Comando: `AT+CMODE=1`  
      Resposta esperada: `OK`
    - **Conectar Apenas ao Dispositivo Emparelhado**  
      Comando: `AT+CMODE=0`  
      Resposta esperada: `OK`

5. **Emparelhamento com Outro Dispositivo Bluetooth**
    - **Iniciar Pesquisa de Dispositivos Bluetooth (Modo Mestre)**  
      Comando: `AT+INQ`  
      Resposta esperada: Lista de endereços de dispositivos.
    - **Conectar a Um Dispositivo Bluetooth (Modo Mestre)**  
      Comando: `AT+LINK=<endereço>`  
      Substitua `<endereço>` pelo endereço do dispositivo detectado na pesquisa.

---

## Resolvendo Problemas Comuns
- **Sem Resposta do Módulo**:  
  - Verifique se o módulo está em **modo AT** (luz piscando lentamente).  
  - A **taxa de baud** no código e no Monitor Serial está correta.  
  - Certifique-se de que o comando `bluetooth.begin()` está correto no código.

---

## Exemplo de Teste Completo
1. Envie `AT` — você deve receber `OK`.
2. Envie `AT+UART` para verificar a taxa de baud.
3. Envie `AT+NAME=SLAVE` para renomear o dispositivo como escravo.
4. Envie `AT+ROLE=0` para configurar como Escravo.
5. Envie `AT+CMODE=1` para se conectar a qualquer dispositivo.

Após todo o processo, cole o código **cubetto.ino** no Arduino IDE e faça o upload para o Arduino.
