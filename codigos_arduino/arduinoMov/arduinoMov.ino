#include <Wire.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

// Motor Variables
const int in1 = 7; // Pin IN1 del L293D para el motor izquierdo
const int in2 = 8; // Pin IN2 del L293D para el motor izquierdo
const int in3 = 10; // Pin IN3 del L293D para el motor derecho
const int in4 = 11; // Pin IN4 del L293D para el motor derecho

const int enA = 6; // Pin ENA del L293D para controlar la velocidad del motor izquierdo
const int enB = 9; // Pin ENB del L293D para controlar la velocidad del motor derecho

const int velocidadMaxima = 255; // Ajusta la velocidad máxima según sea necesario

void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  Serial.println("Setup done.");
}

void loop() {
  Serial.println("Starting..");
  Serial.println("fw..");
  avanzar(10); // Avanza 10 metros
  esperar(2000); // Espera 10 segundos
  Serial.println("girandoDer..");
  girarIzquierda(); // Girar izquierda
  esperar(2000); // Espera 10 segundos
}

void avanzar(int distancia) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  analogWrite(enA, velocidadMaxima);
  analogWrite(enB, velocidadMaxima);
  delay(5000);
  
  detenerMotores();
}

void girarIzquierda() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  analogWrite(enA, velocidadMaxima);
  analogWrite(enB, velocidadMaxima);

  // Ajusta el tiempo necesario para girar a la izquierda
  delay(5000);
  
  detenerMotores();
}

void detenerMotores() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void esperar(unsigned long tiempo) {
  unsigned long inicioTiempo = millis();
  while (millis() - inicioTiempo < tiempo) {
  }
}
