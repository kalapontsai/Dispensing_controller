#include <Wire.h> 
#include <HX711.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int BUTTON = 13; //啟動重量比對和灌膠relay
const int TUNNER = A0;
const int RELAY = 15;
const int scale_factor = 1975; //比例參數，從校正程式中取得
int OFFSET = 1; //補正值

HX711 scale;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TUNNER, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration...");
  delay(1000);
  scale.begin(12, 14); //DT = D6, SCK = D5
  Serial.println("Initializing the scale");
  Serial.println("Before setting up the scale:"); 
  Serial.println(scale.get_units(5), 0);  //未設定比例參數前的數值
  scale.set_scale(scale_factor);       // 設定比例參數
  scale.tare();               // 歸零
  Serial.println("After setting up the scale:"); 
  Serial.println(scale.get_units(5), 0);  //設定比例參數後的數值
  Serial.println("Readings:");  //在這個訊息之前都不要放東西在電子稱上

  digitalWrite(RELAY, LOW);
  digitalWrite(LED_BUILTIN, HIGH); // LED OFF
  lcd.clear();
  lcd.setCursor(0, 0);  //設定游標位置 (字,行)
  lcd.print("Weight: ");
  lcd.setCursor(13, 0);
  lcd.print("g");
  lcd.setCursor(3, 1);  //設定游標位置 (字,行)
  lcd.print("SET : ");
  lcd.setCursor(13, 1);  //設定游標位置 (字,行)
  lcd.print("g");
}

void loop() {
  boolean buttonState = digitalRead(BUTTON);
  Serial.print("Button state : ");
  Serial.println(buttonState);

  float weight = scale.get_units(3) + OFFSET;
  //避免出現負數
  if(weight <= OFFSET){
    weight = 0;
  }
  int TuneValue = analogRead(TUNNER);
  float SetValue = (map(TuneValue,10,1023,1,800)/10.0); //讓插入值為小數一位 
  Serial.print("TuneValue / SetValue = ");
  Serial.print(TuneValue);
  Serial.print(" / ");
  Serial.println(SetValue);
  Serial.print("Weight : ");
  Serial.println(weight);

  if (String(weight,1).length() <= 3) {
    lcd.setCursor(9,0);
    lcd.print(" ");
    lcd.setCursor(10, 0);
    }
  else {
    lcd.setCursor(9, 0);
    }
  lcd.print(weight,1);
  if (String(SetValue,1).length() <= 3) {
    lcd.setCursor(9,1);
    lcd.print(" ");
    lcd.setCursor(10, 1);
    }
  else {
    lcd.setCursor(9, 1);
    }
  lcd.print(SetValue,1);
  if ((buttonState == LOW) && (SetValue > weight)) {
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  } 
  if (SetValue <= weight) {
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  //scale.power_down();             // 進入睡眠模式
  //delay(500);
  //scale.power_up();               // 結束睡眠模式
}
