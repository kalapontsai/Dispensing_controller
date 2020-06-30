#include <Wire.h> 
#include "HX711.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// 接線
const int LED_PIN = 9;
const int RELAY_PIN = 7; // 灌膠機訊號
const int buttonPin = 4; //啟動重量比對和灌膠relay
const int DT_PIN = 6;
const int SCK_PIN = 5;
const int TUNE_PIN = A0;
const int scale_factor = 1963; //比例參數，從校正程式中取得
boolean shoot = false;
int buttonState = 0;

HX711 scale;

void setup() {
  pinMode(LED_PIN,OUTPUT);
  pinMode(TUNE_PIN, INPUT);
  pinMode(buttonPin,INPUT_PULLUP); //enable 內部的上拉電阻
  pinMode(RELAY_PIN,OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration...");
  
  scale.begin(DT_PIN, SCK_PIN);
  Serial.println("Initializing the scale");
  Serial.println("Before setting up the scale:"); 
  Serial.println(scale.get_units(5), 0);  //未設定比例參數前的數值
  scale.set_scale(scale_factor);       // 設定比例參數
  scale.tare();               // 歸零
  Serial.println("After setting up the scale:"); 
  Serial.println(scale.get_units(5), 0);  //設定比例參數後的數值
  Serial.println("Readings:");  //在這個訊息之前都不要放東西在電子稱上

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  int TuneValue = analogRead(TUNE_PIN);
  float weight = scale.get_units(10);
  //避免出現負數
  if(weight<=0){
    weight = 0;
  }
  int SetValue = map(TuneValue,0,1023,0,100);
  // Serial.println(scale.get_units(10), 1); 
  lcd.clear();
  lcd.setCursor(0, 0);  //設定游標位置 (字,行)
  lcd.print("Weight: ");
  lcd.setCursor(14, 0);
  lcd.print("g");
  lcd.setCursor(0, 1);  //設定游標位置 (字,行)
  lcd.print("SET : ");
  lcd.setCursor(10, 1);  //設定游標位置 (字,行)
  lcd.print("g");

  Serial.print("TuneValue / SetValue = ");
  Serial.print(TuneValue);
  Serial.print(" / ");
  Serial.println(SetValue);
  lcd.setCursor(9, 0);
  lcd.print(weight,1);
  lcd.setCursor(6, 1);
  lcd.print(SetValue);
  if ((buttonState == LOW) && (SetValue > weight)) {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } 
  if (SetValue <= weight) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  //scale.power_down();             // 進入睡眠模式
  delay(200);
  //scale.power_up();               // 結束睡眠模式
}
