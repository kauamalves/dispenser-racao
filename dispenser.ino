#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Ultrasonic.h>

#define SERVO1 5
#define ledOn 2

LiquidCrystal_I2C lcd(0x27, 20, 4);     // Initialize the LCD with address 0x27, 20 columns e 4 lines
RTC_DS1307 rtc;                         // Object to manipulate the RTC 
Servo s;                                // Object to manipulate the servo

int servoPos;                           // Position from servo
bool servoExec = false;                 // Flag to indicate if the servo was triggered
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

void setup() {
  Serial.begin(9600);                   // Initialize the serial communication

  if (!rtc.isrunning()) {               // If rtc isn't running, adjust now time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  delay(100);

  lcd.init();                           // Initialize the LCD
  lcd.backlight();                      // Turn on the backlight from LDC
  s.attach(SERVO1);                     // Attach the servo motor to the corresponding pin
  s.write(0);                           // Define the initial pos from servo1
}

void loop() {
  DateTime now = rtc.now();             // Catch now date from RTC

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

  // Verify if is hour to feed and if the servoExec is true
  if (!servoExec && (now.hour() == 10 || now.hour() == 15)) {
    servoExec = true;
  
    // Update the LCD
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

    feeding();
  }

  // If the time is different from the feeding time, reset the servo flag
  if (now.hour() != 10 && now.hour() != 15) {
    servoExec = false;
  }

  delay(3000);  // Wait 3 seconds before the next iteration
}

// Function to open the servo
void feeding(){
  // Open the servo
  for (servoPos = 0; servoPos < 90; servoPos++) {
    s.write(servoPos);
    delay(1);
  }

  delay(3000);  // Wait 3 seconds

  // Close the servo
  for (servoPos = 90; servoPos >= 0; servoPos--) {
    s.write(servoPos);
    delay(1);
  }
}
