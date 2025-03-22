
#include <pins_arduino.h>
#include <PluggableUSB.h>
#include <time.h>
#include <Print.h>
#include <Wire.h>
#include <Arduino.h>
#include <HCSR04.h>
#define TONE_USE_INT
#define TONE_PITCH 440
#include <TonePitch.h>
#include <VibrationMotor.h>
#include <Battery.h>


// Sensor Ultrassônico
UltraSonicDistanceSensor distanceSensor(2, 3);  // Inicia o sensor usando as portas digitais 2 e 3.

// Sensor de Cor
#define PinOUT 8  //Definição do Pino OUT na entrada digital 8
#define PinS2 9   //Definição do Pino S2 na entrada digital 9
#define PinS3 10  //Definição do Pino S3 na entrada digital 10
int red;          //Declaração da variável que representará a cor vermelha
int green;        //Declaração da variável que representará a cor verde
int blue;         // Declaração da variável que representará a cor azul

// Botão de vibração
int PinButtonV = 11;

// Motor de vibração
const int motorPin = 5;
VibrationMotor myVibrationMotor(motorPin);

// Bateria
Battery battery(6000, 8400, A0 );

// Buzzer
const int buzzerPin = 13;



void setup() {

  // Inicializa a porta serial
  Serial.begin(9600);

  // Bateria
  while (!Serial);
  battery.begin(5000, 1.0);  // Inicializa a leitura da bateria

  // Sensor de Cor
  pinMode(PinOUT, INPUT);  //Definição do PinOUT como sendo um pino de entrada
  pinMode(PinS2, OUTPUT);  //Definição do Pino S2 como sendo um pino de saída
  pinMode(PinS3, OUTPUT);  //Definição do Pino S3 como sendo um pino de saída

  // Botão de vibração
  pinMode(PinButtonV, INPUT);  //Definição do PinButtonV como sendo um pino de entrada

  // Motor de vibração
  pinMode(motorPin, OUTPUT);  ////Definição do motorPin como sendo um pino de saída

  // Buzzer
  pinMode(buzzerPin, OUTPUT);  //Definição do buzzerPin como sendo um pino de saída
}


void loop() {

  //Bateria + botão de ligar

 // Serial.print("Battery voltage is ");
  //Serial.print(battery.voltage());
 // Serial.print("mV (");
  Serial.print(battery.level());
  Serial.println("%)");


  // Sensor Ultrasônico
  Serial.println(distanceSensor.measureDistanceCm());  //Faz uma leitura distância em cm
  delay(150);

  if (distanceSensor.measureDistanceCm() < 120) {  // Ativa a vibração em pulsos se identificar algo em 150 cm de proximidade
    myVibrationMotor.pulse(2);
  }


  // Sensor de Cor
  digitalWrite(PinS2, LOW);    // Aciona um valor LOW ao Pino S2
  digitalWrite(PinS3, LOW);    // Aciona um valor LOW ao Pino S3
  red = pulseIn(PinOUT, LOW);  // define red como sendo responsável por ler a informação de pulso LOW do pino out
                               //Serial.print(" R: "); // Mostra a letra R no monitor Serial, representando red
                               //Serial.print(red); // Mostra o valor lido no PulseIn
  delay(15);                   // delay de 15 milissegundos até o próximo comando

  digitalWrite(PinS2, HIGH);     // Aciona um valor high ao Pino S2
  digitalWrite(PinS3, HIGH);     // Aciona um valor high ao Pino S3
  green = pulseIn(PinOUT, LOW);  // define green como sendo responsável por ler a informação de pulso low no pino out
  //Serial.print(" G: "); // Mostra a letra R no monitor Serial, representando green
  // Serial.print(green); // Mostra o valor lido no PulseIn
  delay(15);  // delay de 15 milissegundos até o próximo comando

  digitalWrite(PinS2, LOW);     // Aciona um valor low ao Pino S2
  digitalWrite(PinS3, HIGH);    // Aciona um valor high ao Pino S3
  blue = pulseIn(PinOUT, LOW);  // define blue como sendo responsável por ler a informação de pulso low no pino out
  //Serial.print(" B: "); // Mostra a letra B no monitor Serial, representando blue
  //Serial.println(blue); // Mostra o valor lido no PulseIn
  delay(15);  // delay de 15 milissegundos até o próximo comando

  if (blue < 20 && red < 20 && green < 20) {  //detecção da cor branca e ativa vibração em pulsos
    myVibrationMotor.pulse(3);
    Serial.println("Branco");
  } else if (blue < red && blue < green) {  //detecção da cor azul
    Serial.println("Azul");
  } else if (green < red && green < blue) {  //detecção da cor verde
    Serial.println("Verde");
  } else if (red < blue && red < green && red < 100) {  //detecção da cor vermelha
    Serial.println("Vermelho");
  }

  delay(100);


  // Buzzer
  if (battery.level(10)) {
    tone(13, NOTE_C4, 1000);  //Pino, frequencia, duração do buzzer
    delay(300);
  }

  else if (battery.level(5)) {
    tone(13, NOTE_D4, 1000);  //Pino, frequencia, duração do buzzer
    delay(300);
  }

  else if (battery.level(0)) {
    tone(13, NOTE_A4, 2000);  //Pino, frequencia, duração do buzzer
    delay(300);
  }


  // Botão de Vibração
  if (digitalRead(PinButtonV) == HIGH) {  // Se pressionado, ativa a vibração
    myVibrationMotor.on();
    Serial.println("VIBRANDO");
  }

  else {
    myVibrationMotor.off();
    Serial.println("PARADO");
  }

  delay(300);
}
