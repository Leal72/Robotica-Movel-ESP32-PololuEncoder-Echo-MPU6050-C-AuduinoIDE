#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  Serial.println("MPU6050 inicializado com sucesso!");
}

void loop() {
  int16_t ax, ay, az;

  mpu.getAcceleration(&ax, &ay, &az);

  Serial.print("Acelerômetro: ");
  Serial.print("X = "); Serial.print(ax);
  Serial.print(" Y = "); Serial.print(ay);
  Serial.print(" Z = "); Serial.println(az);

  delay(500);
}
