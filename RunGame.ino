#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2);

const int buttonUp = 2;
const int buttonDown = 3;

int carLane = 1;
int obstacleLane = 0;
int obstaclePos = 16;
int score = 0;

byte carSymbol[8] = {
  B00000, B00000, B01100, B11110, B11111, B01010, B00000, B00000
};

byte obstacleSymbol[8] = {
  B00000, B01110, B11111, B10101, B11111, B01110, B00000, B00000
};

byte roadSymbol[8] = {
  B00000, B00100, B00000, B00100, B00000, B00100, B00000, B00000
};

void setup() {
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, carSymbol);
  lcd.createChar(1, obstacleSymbol);
  lcd.createChar(2, roadSymbol);
}

void loop() {
  // Control del jugador
  if (digitalRead(buttonUp) == LOW) {
    carLane = 0;
    delay(200);
  }
  if (digitalRead(buttonDown) == LOW) {
    carLane = 1;
    delay(200);
  }

  // Movimiento del obstáculo
  obstaclePos--;

  if (obstaclePos < 0) {
    obstaclePos = 16;
    obstacleLane = random(0, 2);
    score++;
  }

  // Dibujar pista
  lcd.clear();
  for (int i = 0; i < 16; i += 3) {
    lcd.setCursor(i, 0);
    lcd.write(byte(2));
    lcd.setCursor(i, 1);
    lcd.write(byte(2));
  }

  // Auto del jugador
  lcd.setCursor(0, carLane);
  lcd.write(byte(0));

  // Obstáculo
  lcd.setCursor(obstaclePos, obstacleLane);
  lcd.write(byte(1));

  // Detectar colisión
  if (obstaclePos == 0 && carLane == obstacleLane) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("COLISION!");
    lcd.setCursor(1, 1);
    lcd.print("Puntaje: " + String(score));
    while (true);
  }

  delay(300);
}