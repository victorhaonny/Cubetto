#include "SoftwareSerial.h"

// Pinos TX e RX para comunicação com o Bluetooth
SoftwareSerial bluetooth(10, 11); // TX, RX (Bluetooth)
int LED1 = 2;
int LED2 = 3; 
int LED3 = 4; 
int LED4 = 5;  

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
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
    char data = bluetooth.read();
    Serial.println(data);  // Exibe a resposta do HC-05

    if (data >= 'A' && data <= 'Z') { // verifica se e letra maiucula coloquei apenas para evitar possiveis ruidos
      if (data == 'F') {
        Serial.println("Anda Pra Frente");
        digitalWrite(LED1, HIGH);
      } else if (data == 'C') {
        Serial.println("Chama a Funcao");
        digitalWrite(LED2, HIGH);
      } else if (data == 'D') {
        Serial.println("Vira a direita");
        digitalWrite(LED3, HIGH);
      } else if (data == 'E') {
        Serial.println("Vira a esquerda");
        digitalWrite(LED4, HIGH);
      } else {
        Serial.println("Sinal nao encontrado");
      }
    }
  }
    delay(300);
    digitalWrite(LED1, 0);
    digitalWrite(LED2, 0);
    digitalWrite(LED3, 0);
    digitalWrite(LED4, 0);
}
