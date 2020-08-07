/*
  本範例為HX711的校正程式
  
  需先安裝Bogdan Necula的HX711函式庫
  https://github.com/bogde/HX711
  
  傑森創工製作
  https://blog.jmaker.com.tw/hx711_set/
   */
#include "HX711.h"

// HX711 接線設定
const int DT_PIN = 12;
const int SCK_PIN = 14;
const int sample_weight = 64.8;  //基準物品的真實重量(公克)
// use Product 99D (2x2 case) for sample, scale factor is 1963

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale();  // 開始取得比例參數
  scale.tare();
  Serial.println("Nothing on it.");
  Serial.println(scale.get_units(10));
  Serial.println("Please put sapmple object on it..."); //提示放上基準物品
  
}

void loop() {
  float current_weight=scale.get_units(10);  // 取得10次數值的平均
  float scale_factor=(current_weight/sample_weight);
  Serial.print("Scale number:  ");
  Serial.println(scale_factor,0);  // 顯示比例參數，記起來，以便用在正式的程式中
}
