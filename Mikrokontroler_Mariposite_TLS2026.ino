#include <ESP32Servo.h>
#include <DHT.h>

const int PIR_PIN = 27;
const int SERVO_PIN = 13;
const int LED_PIN = 23;
const int DHT_PIN = 4;

#define DHTTYPE DHT22

Servo myServo;
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  dht.begin();

  myServo.write(0);

  Serial.println("Sistem Security & Monitoring Suhu Aktif...");
}

void loop() {
  int gerakan = digitalRead(PIR_PIN);
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca dari sensor DHT22! Periksa kabel.");
    delay(2000);
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C | Gerakan: ");
  Serial.println(gerakan == HIGH ? "TERDETEKSI" : "Tidak Ada");

  if (gerakan == HIGH && suhu >= 37.0) {
    Serial.println(" BAHAYA: Suhu Tinggi & Gerakan Terdeteksi Bersamaan!");

    myServo.write(90);

    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      delay(200);
    }

  } else {
    Serial.println("Kondisi Normal (Aman)...");

    myServo.write(0);
    digitalWrite(LED_PIN, LOW);
  }

  delay(2000);
}