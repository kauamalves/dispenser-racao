#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "Ultrasonic.h"

#define SERVO1 5
#define echoPin 4
#define trigPin 3
#define ledOn 2

LiquidCrystal_I2C lcd(0x27, 20, 4);     // Inicializa o LCD com endereço 0x27, 20 colunas e 4 linhas
RTC_DS1307 rtc;                         // Objeto para manipulação do módulo RTC
Servo s1;                               // Objeto para manipulação do servo motor
Ultrasonic ultrasonic(trigPin, echoPin);// Objeto para manipulação do sensor ultrassônico

int distancia;                          // Variável para armazenar a distância medida pelo sensor ultrassônico
int servoPos;                           // Posição do servo motor
bool servoExecutado = false;            // Flag para indicar se o servo motor já foi acionado
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

void setup() {
  pinMode(echoPin, INPUT);              // Define o pino echo como INPUT
  pinMode(trigPin, OUTPUT);             // Define o pino trig como OUTPUT
  pinMode(ledOn, OUTPUT);               // Define o pino do led ligado como OUTPUT

  Serial.begin(9600);                   // Inicializa a comunicação serial
  hcsr04();                             // Chama a função para inicializar o sensor ultrassônico

  if (!rtc.begin()) {                   // Inicializa o RTC e verifica se foi bem-sucedido
    Serial.println("DS1307 não encontrado");
    while (1);
  }
  if (!rtc.isrunning()) {               // Se o RTC não está rodando, ajusta a data e hora
    Serial.println("DS1307 rodando!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  delay(100);

  lcd.init();                           // Inicializa o LCD
  lcd.backlight();                      // Liga a luz de fundo do LCD
  s1.attach(SERVO1);                    // Anexa o servo motor ao pino correspondente
  Serial.begin(9600);                   // Inicializa novamente a comunicação serial
  s1.write(0);                          // Define a posição inicial do servo motor
}

void loop() {
  DateTime now = rtc.now();             // Obtém a data e hora atual do RTC
  
  // Exibe a hora no LCD
  lcd.setCursor(3, 0);
  lcd.print("LIA - ");
  lcd.setCursor(7, 0);
  lcd.print(now.hour());
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(10, 0);
  lcd.print(now.minute());
  lcd.setCursor(1, 1);
  lcd.print("Iniciando sensores...");
  delay(500);
  
  // Limpa o LCD
  digitalWrite(ledOn, HIGH);
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

  // Verifica se é o horário de alimentar e se o servo ainda não foi acionado
  if (!servoExecutado && distancia < 15 && (now.hour() == 11 || now.hour() == 16)) {
    servoExecutado = true;
  
    // Atualiza o LCD durante a alimentação
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

    // Move o servo motor para alimentar
    for (servoPos = 0; servoPos < 90; servoPos++) {
      s1.write(servoPos);
      delay(1);
    }

    delay(4000);  // Espera 4 segundos após alimentar

    // Retorna o servo motor à posição inicial
    for (servoPos = 90; servoPos >= 0; servoPos--) {
      s1.write(servoPos);
      delay(1);
    }
  }

  // Se não estiver no horário de alimentar, redefine a flag do servo
  if (now.hour() != 11 && now.hour() != 16) {
    servoExecutado = false;
  }

  delay(3000);  // Aguarda 3 segundos antes da próxima iteração
}

// Função para inicializar o sensor ultrassônico
void hcsr04(){
  digitalWrite(trigPin, LOW);     // Define o pino trig como LOW
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);    // Define o pino trig como HIGH
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);     // Define o pino trig como LOW novamente

  distancia = ultrasonic.read();  // Faz a leitura da distância em centímetros
  delay(1000);                     // Espera 1 segundo antes de fazer outra leitura
}
