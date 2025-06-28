#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "melodia.h"
#include "notas.h"

LiquidCrystal_I2C lcd(0x3f, 16, 2);

const int buttonUp = 2;
const int buttonDown = 3;
const int buzzerPin = 4;

int carLane = 1;
int obstacleLane = 0;
int obstaclePos1 = 16;
int obstaclePos2 = -1;
int score = 0;
int level = 1;
bool gameOver = false;
int gameSpeed = 300;

int notaActual = 0;
unsigned long tiempoNotaAnterior = 0;
bool sonidoActivo = true;

byte carSymbol[8] = {
  B00000, B00000, B01100, B11110, B11111, B01010, B00000, B00000
};

byte obstacleSymbol[8] = {
  B00000, B01110, B11111, B10101, B11111, B01110, B00000, B00000
};

byte roadSymbol[8] = {
  B00000, B00100, B00000, B00100, B00000, B00100, B00000, B00000
};

void showMenu();
void resetGame();
void reproducirMelodia();
void reproducirMelodiaFondo();

void setup() {
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, carSymbol);
  lcd.createChar(1, obstacleSymbol);
  lcd.createChar(2, roadSymbol);

  showMenu();
}

void loop() {
  if (gameOver) {
    sonidoActivo = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Puntaje: " + String(score));
    reproducirMelodia();
    while (digitalRead(buttonUp) == HIGH && digitalRead(buttonDown) == HIGH) {
      delay(50);
    }
    resetGame();
  }

  if (digitalRead(buttonUp) == LOW) {
    carLane = 0;
    delay(200);
  }
  if (digitalRead(buttonDown) == LOW) {
    carLane = 1;
    delay(200);
  }

  obstaclePos1--;
  if (obstaclePos2 >= 0) obstaclePos2--;

  if (obstaclePos1 < 0) {
    obstaclePos1 = 16;
    obstacleLane = random(0, 2);
    score++;

    if (score % 5 == 0 && gameSpeed > 80) {
      gameSpeed -= 10;
      level++;
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("NIVEL " + String(level));
      tone(buzzerPin, 1000, 200);
      delay(800);
    }

    if (random(0, 3) == 1) {
      obstaclePos2 = obstaclePos1 - random(2, 7);
    } else {
      obstaclePos2 = -1;
    }
  }

  lcd.clear();
  for (int i = 0; i < 16; i += 3) {
    lcd.setCursor(i, 0);
    lcd.write(byte(2));
    lcd.setCursor(i, 1);
    lcd.write(byte(2));
  }

  lcd.setCursor(0, carLane);
  lcd.write(byte(0));
  lcd.setCursor(obstaclePos1, obstacleLane);
  lcd.write(byte(1));

  if (obstaclePos2 >= 0) {
    lcd.setCursor(obstaclePos2, obstacleLane);
    lcd.write(byte(1));
  }

  if ((obstaclePos1 == 0 && carLane == obstacleLane) ||
      (obstaclePos2 == 0 && carLane == obstacleLane)) {
    gameOver = true;
  }

  reproducirMelodiaFondo();
  delay(gameSpeed);
}

// 🎬 Menú musical en bucle
void showMenu() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("AUTO RUN!");
  lcd.setCursor(2, 1);
  lcd.print("Presiona btn!");

  int nota = 0;
  unsigned long previousMillis = 0;

  while (digitalRead(buttonUp) == HIGH && digitalRead(buttonDown) == HIGH) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= melodia[nota][1] * 1.3) {
      int tono = melodia[nota][0];
      int duracion = melodia[nota][1];
      if (tono != REST) {
        tone(buzzerPin, tono, duracion);
      }
      previousMillis = currentMillis;
      delay(duracion);
      noTone(buzzerPin);
      nota++;
      if (nota >= numNotas) nota = 0;
    }
  }

  noTone(buzzerPin);
}

void resetGame() {
  gameOver = false;
  score = 0;
  level = 1;
  carLane = 1;
  obstaclePos1 = 16;
  obstaclePos2 = -1;
  gameSpeed = 300;
  sonidoActivo = true;
  notaActual = 0;
  tiempoNotaAnterior = millis();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nuevo Juego!");
  delay(1000);
}

void reproducirMelodia() {
  for (int i = 0; i < numNotas; i++) {
    int tono = melodia[i][0];
    int duracion = melodia[i][1];
    if (tono != REST) {
      tone(buzzerPin, tono, duracion);
    }
    delay(duracion * 1.3);
    noTone(buzzerPin);
  }
}

void reproducirMelodiaFondo() {
  if (!sonidoActivo) return;

  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoNotaAnterior >= melodia[notaActual][1] * 1.3) {
    int tono = melodia[notaActual][0];
    int duracion = melodia[notaActual][1];

    if (tono != REST) {
      tone(buzzerPin, tono, duracion);
    }

    tiempoNotaAnterior = tiempoActual;
    notaActual++;
    if (notaActual >= numNotas) notaActual = 0;
    noTone(buzzerPin);
  }
}