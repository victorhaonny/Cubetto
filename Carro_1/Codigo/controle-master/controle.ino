#include <SoftwareSerial.h>

// Definição dos pinos dos botões
const int buttonForward = 2;
const int buttonLeft = 3;
const int buttonRight = 4;
const int buttonStart = 5;

// LED único para indicação
const int ledIndicator = 6;

SoftwareSerial BTSerial(10, 11);  // RX, TX para o Bluetooth

void setup() {
  // Configuração dos botões
  pinMode(buttonForward, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonStart, INPUT_PULLUP);

  // Configuração do LED
  pinMode(ledIndicator, OUTPUT);

  BTSerial.begin(9600);  // Inicia a comunicação Bluetooth
  Serial.begin(9600);    // Inicia a comunicação Serial
}

void loop() {
  // Verifica o botão Forward
  if (digitalRead(buttonForward) == LOW) {
    BTSerial.write('F');  // Envia comando para mover para frente
    digitalWrite(ledIndicator, HIGH);  // Acende o LED indicativo
    delay(300);  // Evita múltiplos comandos rápidos
    digitalWrite(ledIndicator, LOW);  // Apaga o LED
    while (digitalRead(buttonForward) == LOW);  // Espera o botão ser liberado
  }

  // Verifica o botão Left
  if (digitalRead(buttonLeft) == LOW) {
    BTSerial.write('L');  // Envia comando para virar à esquerda
    digitalWrite(ledIndicator, HIGH);  // Acende o LED indicativo
    delay(300);  // Evita múltiplos comandos rápidos
    digitalWrite(ledIndicator, LOW);  // Apaga o LED
    while (digitalRead(buttonLeft) == LOW);  // Espera o botão ser liberado
  }

  // Verifica o botão Right
  if (digitalRead(buttonRight) == LOW) {
    BTSerial.write('R');  // Envia comando para virar à direita
    digitalWrite(ledIndicator, HIGH);  // Acende o LED indicativo
    delay(300);  // Evita múltiplos comandos rápidos
    digitalWrite(ledIndicator, LOW);  // Apaga o LED
    while (digitalRead(buttonRight) == LOW);  // Espera o botão ser liberado
  }

  // Verifica o botão Start
  if (digitalRead(buttonStart) == LOW) {
    digitalWrite(ledIndicator, HIGH);  // Acende o LED indicativo
    BTSerial.write('S');  // Envia comando para iniciar a execução
    delay(300);  // Evita múltiplos comandos rápidos
    digitalWrite(ledIndicator, LOW);  // Apaga o LED
    while (digitalRead(buttonStart) == LOW);  // Espera o botão ser liberado
  }

  delay(100);  // Pequeno delay para estabilidade
}
