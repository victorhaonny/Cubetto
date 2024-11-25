#include <SoftwareSerial.h>
SoftwareSerial bluetooth(10,11);

//String command = "";


/*int IN1 = 4;
int IN2 = 5;
int IN3 = 6;
int IN4 = 7;*/

int Count = 0;
//String Cmd = "";
int Cmd[30] ={0};
//char Cmd;
//int Cmd;

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
 
  Serial.println("Digite os comandos AT :");  
  //Inicia a serial configurada nas portas 10 e 11
  bluetooth.begin(9600);  
  //Define os pinos como saida
  /*pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);*/
}

void loop() {
  //Serial.println("nada");
  // send data only when you receive data:
  if (bluetooth.available() > 0) {
    //while (bluetooth.available() > 0) {
    // read the incoming byte:
    Cmd[Count] = bluetooth.read();
    //Serial.println(Count);
    //Cmd = (char)bluetooth.read();
    //Cmd = bluetooth.read();
    Serial.print(Cmd[Count]);
    //Serial.println(Cmd);
    //if (Cmd == 'm'){
      //Serial.println("m");
//   }
    Count++;
   if (Count == 16)
    {
      for (int i = 0; i <16; i++)
        {
          if (Cmd[i] == 2)
            Serial.print("laranja ");
          else if (Cmd[i] == 3)
            Serial.print("amarelo ");
          else if (Cmd[i] == 1)
            Serial.print("azul ");
          else if (Cmd[i] == 4)
            Serial.print("verd ");
          //else if (Cmd[i] == 0)
            //Serial.println("zero");
        }
      Count = 0;
      Serial.println("contador zero ");
    }
    //Count = 0;
  //}
  }
}
    
 

