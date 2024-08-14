#define DEBUG 1

#define DEBOUNCE_DELAY 200
#define COINSENSOR_1 PB10
#define COINSENSOR_2 PB9
#define COIN1_MULTIPLIER 1
#define COIN2_MULTIPLIER 0.5


#include <Arduino.h>
#include <LiquidCrystal.h>

uint8_t coin1Count = 0;
uint8_t coin2Count = 0;
uint32_t lastUpdate_1 = 0;
uint32_t lastUpdate_2 = 0;

float totalAmount = 0;
float coin1Amount = 0;
float coin2Amount = 0;

LiquidCrystal lcd(PA10, PB3, PA7, PA6, PA5, PA3);

void updateDisplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("0.5$:" + String(coin1Count) + " 1$:" + String(coin2Count));
  lcd.setCursor(0, 1);
  lcd.print("Total: " + String(totalAmount) +"$" );

  Serial.println("Coin 1: " + String(coin1Count) + "\t Coin 2: " + String(coin2Count) + "\t Total: " + String(totalAmount));
}

void amountUpdate(){
  coin1Amount = coin1Count * COIN1_MULTIPLIER;
  coin2Amount = coin2Count * COIN2_MULTIPLIER;
  totalAmount = coin1Amount + coin2Amount;
  updateDisplay();
  if (DEBUG){ Serial.println("Coin1 Amount: " + String(coin1Amount) + "\t Conin2 Amount: " + String(coin2Amount) + "\t Total Amount: " + String(totalAmount));}
}

void readCoinSensor() {
  if (digitalRead(COINSENSOR_1) == LOW) {
    if (millis() - lastUpdate_1 < DEBOUNCE_DELAY) {
      return;
    }
    coin1Count++;
    amountUpdate();
    lastUpdate_1 = millis();
    if (DEBUG){Serial.println("Coin 1 inserted");}
  }
  if (digitalRead(COINSENSOR_2) == LOW) {
    if (millis() - lastUpdate_2 < DEBOUNCE_DELAY) {
      return;
    }
    coin2Count++;
    amountUpdate();
    lastUpdate_2 = millis();
    if (DEBUG){Serial.println("Coin 2 inserted");}
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  if (DEBUG){lcd.print("INSERT COIN");}
  pinMode(COINSENSOR_1, INPUT_PULLUP);
  pinMode(COINSENSOR_2, INPUT_PULLUP);
  
}

void loop() {
  readCoinSensor();
}

