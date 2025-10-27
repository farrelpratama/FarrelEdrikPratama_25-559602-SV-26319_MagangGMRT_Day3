// Nama : Farrel Edrik Pratama
// NIM : 25/559602/SV/26319

#include <Wire.h>
#include <ESP32Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// === Inisialisasi objek sensor dan servo ===
Adafruit_MPU6050 mpu;
Servo servo1, servo2, servo3, servo4, servo5;

const int pirPin = 33; // PIR sensor
bool motionDetected = false;

// === Pin Servo ===
const int servoPins[5] = {13, 12, 14, 27, 26};
int initialPos = 90;  // posisi tegak lurus

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Inisialisasi MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 tidak terdeteksi!");
    while (1);
  }

  // Inisialisasi servo
  servo1.attach(servoPins[0]);
  servo2.attach(servoPins[1]);
  servo3.attach(servoPins[2]);
  servo4.attach(servoPins[3]);
  servo5.attach(servoPins[4]);

  // Set posisi awal semua servo
  for (int i = 0; i < 5; i++) {
    Servo *srv[] = {&servo1, &servo2, &servo3, &servo4, &servo5};
    srv[i]->write(initialPos);
  }

  // PIR sensor setup
  pinMode(pirPin, INPUT);

  Serial.println("Sistem siap. Menunggu data MPU6050...");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // --- simulasi pembacaan sudut (dari slider di Wokwi) ---
  float roll = a.acceleration.x;   // slider roll
  float pitch = a.acceleration.y;  // slider pitch
  float yaw = a.acceleration.z;    // slider yaw

  // tampilkan data
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(" | Pitch: "); Serial.print(pitch);
  Serial.print(" | Yaw: "); Serial.println(yaw);

  // --- Kondisi PIR ---
  motionDetected = digitalRead(pirPin);

  if (motionDetected) {
    Serial.println("Gerakan terdeteksi! Semua servo aktif!");
    for (int i = 0; i < 5; i++) {
      Servo *srv[] = {&servo1, &servo2, &servo3, &servo4, &servo5};
      srv[i]->write(45);  // posisi bebas yang kamu tentukan
    }
    delay(1000);
    for (int i = 0; i < 5; i++) {
      Servo *srv[] = {&servo1, &servo2, &servo3, &servo4, &servo5};
      srv[i]->write(initialPos);
    }
    delay(500);
  }

  // --- Roll control ---
  if (roll > 5) {  // rotasi ke arah positif
    servo1.write(initialPos - abs(roll));
    servo2.write(initialPos + abs(roll));
  } else if (roll < -5) { // rotasi negatif
    servo1.write(initialPos + abs(roll));
    servo2.write(initialPos - abs(roll));
  } else {
    servo1.write(initialPos);
    servo2.write(initialPos);
  }

  // --- Pitch control ---
  if (pitch > 5) {  // rotasi ke arah positif
    servo3.write(initialPos + abs(pitch));
    servo4.write(initialPos + abs(pitch));
  } else if (pitch < -5) { // rotasi negatif
    servo3.write(initialPos - abs(pitch));
    servo4.write(initialPos - abs(pitch));
  } else {
    servo3.write(initialPos);
    servo4.write(initialPos);
  }

  // --- Yaw control ---
  if (abs(yaw) > 5) {
    servo5.write(initialPos + yaw);
  } else {
    delay(1000); // diam 1 detik
    servo5.write(initialPos);
  }

  delay(200);
}
