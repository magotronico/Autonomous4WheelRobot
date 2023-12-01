#include <Wire.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

const int in1 = 2; // Pin IN1 del L298N para el motor izquierdo
const int in2 = 3; // Pin IN2 del L298N para el motor izquierdo
const int in3 = 4; // Pin IN3 del L298N para el motor derecho
const int in4 = 5; // Pin IN4 del L298N para el motor derecho

const int enA = 6; // Pin ENA del L298N para controlar la velocidad del motor izquierdo
const int enB = 9; // Pin ENB del L298N para controlar la velocidad del motor derecho

const int velocidadMaxima = 255; // Ajusta la velocidad máxima según sea necesario

void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
}

void loop() {
  avanzar(10); // Avanza 10 metros
  esperar(10000); // Espera 10 segundos
  girarIzquierda(); // Girar izquierda
  avanzar(10); // Avanza 10 metros
  esperar(10000); // Espera 10 segundos
 girarIzquierda(); // Girar izquierda
avanzar(10); // Avanza 10 metros
 esperar(10000); // Espera 10 segundos
girarIzquierda(); // Girar izquierda
avanzar(10); // Avanza 10 metros
}

void avanzar(int distancia) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
  analogWrite(enA, velocidadMaxima);
  analogWrite(enB, velocidadMaxima);
  delay(500);
  
  detenerMotores();
}

void girarIzquierda() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
  analogWrite(enA, velocidadMaxima);
  analogWrite(enB, velocidadMaxima);

  // Ajusta el tiempo necesario para girar a la izquierda
  delay(500);
  
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
