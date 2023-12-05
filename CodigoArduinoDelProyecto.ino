#include <Servo.h>
#include <SoftwareSerial.h>


const byte rxPin = 2;
const byte txPin = 3;


Servo servo;
int pos = 0;
int posIncrement = 1; // Incremento para el movimiento gradual del servo
int servoDelay = 20;  // Retardo para el movimiento lento del servo


int speedPin = 11;
int dirPin1 = 12;
int dirPin2 = 13;
int speedMotor = 255;


unsigned long tiempo1 = 0;
bool servoact = false;
int valor = 0;


SoftwareSerial mySerial(rxPin, txPin);


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);


  servo.attach(9);
  pinMode(speedPin, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
}


void loop() {
  unsigned long currentMillis = millis();


  if (mySerial.available() > 0) {
    valor = mySerial.parseInt(); // Lee el valor enviado por Bluetooth


    if (valor > 0) {
      tiempo1 = currentMillis + (valor * 1000); // Calcula el tiempo de finalización
      Serial.print("Tiempo estimado: ");
      Serial.println(valor);


      digitalWrite(dirPin1, LOW);
      digitalWrite(dirPin2, HIGH);
      analogWrite(speedPin, speedMotor);


      servo.attach(9); // Adjuntar el servo
      servo.write(60); // Posición inicial del servo
      pos = 60; // Establecer la posición inicial


      servoact = true;
    }
  }


  // Control del servo mientras está activo
  if (servoact) {
    if (currentMillis < tiempo1) {
      if (pos <= 60 && pos >= 0) {
        pos += posIncrement;
        servo.write(pos);
        delay(servoDelay); // Retardo para el movimiento lento del servo
      } else {
        posIncrement = -posIncrement; // Invertir la dirección del movimiento
        pos += posIncrement;
        servo.write(pos);
        delay(servoDelay); // Retardo para el movimiento lento del servo
      }
    } else {
      // Detener el motor DC y el servo al finalizar el tiempo
      analogWrite(speedPin, 0);
      servo.detach();
      servoact = false;
      valor = 0;
      tiempo1 = 0;
    }
  }
}
