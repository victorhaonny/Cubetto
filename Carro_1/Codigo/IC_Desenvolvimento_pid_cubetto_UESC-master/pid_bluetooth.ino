#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial BTSerial(2, 3);

// Definição dos pinos do motor
const int IN1 = 10;
const int IN2 = 9;
const int IN3 = 5;
const int IN4 = 6;

const int fullSpeed = 255;  // Velocidade máxima para analogWrite
const int turnSpeed = 20;
const int leftTurnSpeed = 115;  // Velocidade reduzida para viradas suaves
const int turnFullSpeed = 140;  // Velocidade mais alta para viradas mais rápidas

const int encoderRightPin = A0;  // Pino analógico usado para conectar o encoder direito
const int encoderLeftPin = A1;   // Pino analógico usado para conectar o encoder esquerdo

// Variáveis do PID
double setpoint = 150;  // Velocidade desejada
double measuredSpeedRight, measuredSpeedLeft;
double errorRight, errorLeft;
double integralRight = 0, integralLeft = 0;
double derivativeRight, derivativeLeft;
double previousErrorRight = 0, previousErrorLeft = 0;

// Coeficientes PID
double Kp = 1.0, Ki = 0.1, Kd = 0.05;

// Controle de comandos
const int maxCommands = 16;
char commandStack[maxCommands];
int stackPointer = 0;

bool isMovingForward = false;  // Estado de movimento para frente

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  Serial.begin(9600);    // Inicia a comunicação Serial com o computador
  BTSerial.begin(9600);  // Inicia a comunicação BTSerial
}

void loop() {
  if (BTSerial.available()) {
    char command = BTSerial.read();
    if (command == 'S') {
      executeCommands();
      stackPointer = 0;
    } else if (stackPointer < maxCommands) {
      commandStack[stackPointer++] = command;
    }
  }

  if (isMovingForward) {
    controlMotors();
  }

  delay(100);  // Delay para estabilidade
}

void Stop() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
  isMovingForward = false;
}



void executeCommands() {
  for (int i = 0; i < stackPointer; i++) {
    executeCommand(commandStack[i]);
    delay(1000);
  }
}

void executeCommand(char command) {
  switch (command) {
    case 'F':
      Foward();
      delay(1000);
      Stop();
      break;
    case 'L':
      Left();
      delay(1000);
      Stop();
      break;
    case 'R':
      Rigth();
      delay(1000);
      Stop();
      break;
    case 'S':
      Stop();
      delay(1000);
      break;
  }
}

void Foward() {
  isMovingForward = true;
  analogWrite(IN1, fullSpeed);
  analogWrite(IN2, 0);
  analogWrite(IN3, fullSpeed);
  analogWrite(IN4, 0);
}


void Left() {
  analogWrite(IN1, leftTurnSpeed);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, turnSpeed);
}


void Rigth() {
  analogWrite(IN1, 0);
  analogWrite(IN2, turnSpeed);
  analogWrite(IN3, turnFullSpeed);
  analogWrite(IN4, 0);
}


void controlMotors() {
  measuredSpeedRight = analogRead(encoderRightPin);
  measuredSpeedLeft = analogRead(encoderLeftPin);

  errorRight = setpoint - measuredSpeedRight;
  integralRight += errorRight;
  derivativeRight = errorRight - previousErrorRight;

  double outputRight = Kp * errorRight + Ki * integralRight + Kd * derivativeRight;

  errorLeft = setpoint - measuredSpeedLeft;
  integralLeft += errorLeft;
  derivativeLeft = errorLeft - previousErrorLeft;

  double outputLeft = Kp * errorLeft + Ki * integralLeft + Kd * derivativeLeft;

  analogWrite(IN1, constrain(outputRight, 0, 255));
  analogWrite(IN3, constrain(outputLeft, 0, 255));

  previousErrorRight = errorRight;
  previousErrorLeft = errorLeft;
}
