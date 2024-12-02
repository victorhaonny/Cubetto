#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial bluetooth(10,11);
//SoftwareSerial BTSerial(2, 3);

//String command = "";


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
const int maxCommands = 16; // mude pra 30 se for preciso
int Cmd[maxCommands] = {0};
int Count = 0;
bool isMovingForward = false;  // Estado de movimento para frente

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
 
  //Serial.println("Digite os comandos AT :");  
  //Inicia a serial configurada nas portas 10 e 11
  bluetooth.begin(9600);
    
  //Define os pinos como saida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  if (bluetooth.available() > 0) {
    int command = bluetooth.read();

    if(Count < maxCommands){
      Cmd[Count] = command;
      Serial.print(Cmd[Count]);
      Count++;
    }else{
      Count = 0;
    }

   if (Count == maxCommands)
    {
      Serial.println("Executando comandos...");
      processCommands();
    }
    
  // Controle contínuo dos motores enquanto em movimento
  if (isMovingForward) {
    controlMotors();
  }

  delay(100);  
  }
}

// Mapeia comandos para ações
void executeCommand(int command) {
  switch (command) {
    case 1:
      Serial.println("Ação: Azul (Foward)");
      Foward();
      delay(1000);
      Stop();
      break;
    case 2:
      Serial.println("Ação: Laranja (Left)");
      Left();
      delay(1000);
      Stop();
      break;
    case 3:
      Serial.println("Ação: Amarelo (Rigth)");
      Rigth();
      delay(1000);
      Stop();
      break;
    case 4:
      Serial.println("Ação: Verde (Stop)");
      Stop();
      delay(1000);
      break;
    default:
      Serial.println("Ação inválida!");
      break;
  }
}

// Exibe cores correspondentes aos comandos e executa as ações
void processCommands() {
  for (int i = 0; i < Count; i++) {
    if (Cmd[i] == 2) {
      Serial.print("laranja ");
    } else if (Cmd[i] == 3) {
      Serial.print("amarelo ");
    } else if (Cmd[i] == 1) {
      Serial.print("azul ");
    } else if (Cmd[i] == 4) {
      Serial.print("verde ");
    } else {
      Serial.print("desconhecido ");
    }
    executeCommand(Cmd[i]);
  }
  Serial.println();
  Count = 0;  // Reinicia o contador após processar os comandos
}

// Funções de controle do motor
void Stop() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
  isMovingForward = false;
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

// Controle PID para ajustar os motores
void controlMotors() {
  // Lê os valores dos encoders
  measuredSpeedRight = analogRead(encoderRightPin);
  measuredSpeedLeft = analogRead(encoderLeftPin);

  // Calcula o erro para os motores
  errorRight = setpoint - measuredSpeedRight;
  integralRight += errorRight;
  derivativeRight = errorRight - previousErrorRight;

  double outputRight = Kp * errorRight + Ki * integralRight + Kd * derivativeRight;

  errorLeft = setpoint - measuredSpeedLeft;
  integralLeft += errorLeft;
  derivativeLeft = errorLeft - previousErrorLeft;

  double outputLeft = Kp * errorLeft + Ki * integralLeft + Kd * derivativeLeft;

  // Ajusta as saídas dos motores
  analogWrite(IN1, constrain(outputRight, 0, 255));
  analogWrite(IN3, constrain(outputLeft, 0, 255));

  // Atualiza os erros anteriores
  previousErrorRight = errorRight;
  previousErrorLeft = errorLeft;
}
 