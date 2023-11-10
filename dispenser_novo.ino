#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define SERVO1 5

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;
Servo s1;
int servoPos;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
bool servoExecutado = false;



void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("DS1307 não encontrado");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("DS1307 rodando!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  delay(100);

  lcd.init();
  lcd.backlight();
  s1.attach(SERVO1);
  Serial.begin(9600);
  s1.write(0);
}

void loop() {
  DateTime now = rtc.now();

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("LIA - ");
    lcd.setCursor(7, 0);
    lcd.print(now.hour());
    lcd.setCursor(9, 0);
    lcd.print(":");
    lcd.setCursor(10, 0);
    lcd.print(now.minute());

    lcd.setCursor(1, 1);
    lcd.print("Pote preenchido");

  if (!servoExecutado && (now.hour() == 11 || now.hour() == 16)) {
    servoExecutado = true;
  
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("LIA - ");
    lcd.setCursor(7, 0);
    lcd.print(now.hour());
    lcd.setCursor(9, 0);
    lcd.print(":");
    lcd.setCursor(10, 0);
    lcd.print(now.minute());
    lcd.setCursor(2, 1);
    lcd.print("Alimentando");

    for (servoPos = 0; servoPos < 90; servoPos++) {
      s1.write(servoPos);
      delay(1);
    }

    delay(4000);

    for (servoPos = 90; servoPos >= 0; servoPos--) {
      s1.write(servoPos);
      delay(1);
    }
  }

  if (now.hour() == 0 && now.minute() == 0) {
    servoExecutado = false;
  }

  delay(3000);  // Ajuste o tempo de espera conforme necessário
}
