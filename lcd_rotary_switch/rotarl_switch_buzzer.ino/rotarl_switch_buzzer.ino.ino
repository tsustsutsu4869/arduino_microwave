
#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"
//#include "pitches.h"


//ロータリーエンコーダ制御用の定数
#define ADC_REF 5//reference voltage of ADC is 5v.If the Vcc switch on the seeeduino
         //board switches to 3V3, the ADC_REF should be 3.3
#define GROVE_VCC 5//VCC of the grove interface is normally 5v
#define FULL_ANGLE 300//full value of the rotary angle is 300 degrees



Servo Motor;             // Motorという名前でプログラム内で使いますという宣言
int buttonPin = 2;       // ボタンのコネクタ番号（D2につないだ場合）
int ledPin_1 = 4;       // LEDのコネクタ番号（D4につないだ場合）
int ledPin_2 = 8;       // LEDのコネクタ番号（D4につないだ場合）
int buzzerPin = 6;        // ブザーのコネクタ番号（D6につないだ場合）
const int rotaryPin = A0;                    // rotary angle sensor connect to A0
//int motorPin = 8;        // モータのコネクタ番号（D8につないだ場合）
int shaft = 90;          // モータの角度を記憶する変数（90度から開始）


rgb_lcd lcd;

const int colorR = 255;
const int colorG = 45;
const int colorB = 0;

//タイマーの値
const int TIME_VALUE = 6;
//1分間（ミリ秒）
const unsigned long ONE_SECOND = 1000L;  //1秒
//-----------------------
// グローバル変数の定義
//-----------------------
//unsigned long G_time = TIME_VALUE;                   //タイマーの時間
unsigned long G_start_time;                          //スタートボタンを押してからの時間
unsigned long G_time;

char clear_str=' ';
int flag_end = 0;
unsigned int flag_decide_time = 0;

//ブザーの音程，リズム，テンポの定義
int length = 12; // the number of notes
char notes[] = "gfg gfg gfg "; // a space represents a rest
long beats[] = { 1.2, 1.2, 1.2, 1, 1.2, 1.2, 1.2, 1, 1.2, 1.2, 1.2, 1};
int tempo = 150;


int var = 0; //変数varを設定
int old_var = 0; //変数old_varを設定
int buttonState = 0; //変数stateを設定
int pushed_count=0;

void setup() {
  Serial.begin(9600);                         // set the serial communication frequency at 9600 bits per sec
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);

    // Print a message to the LCD.
    lcd.setCursor(9, 0);
    lcd.print("seconds");

    pinMode(buzzerPin, OUTPUT);  // ブザーをOUTPUTとして設定
    pinMode(ledPin_1, OUTPUT);  // led1をOUTPUTとして設定
    pinMode(ledPin_2, OUTPUT);  // led2をOUTPUTとして設定
    
    pinMode(rotaryPin, INPUT);
    pinMode(buttonPin, INPUT);  // ポタンをINPUTとして設定
    

    delay(1000);
}


void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432/4, 1275/4, 1136, 1014, 956 };  //1275
//    int tones[] = { 1915, 1700, 1519, 698, 784, 1136, 1014, 956 };  //1275

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}


int getDegree(){
  int sensor_value = analogRead(rotaryPin);
  float voltage;
  voltage = (float)sensor_value*ADC_REF/1023;
  float degrees = (voltage*FULL_ANGLE)/GROVE_VCC;
  return degrees;
}


void judge_switch(){
  if (pushed_count == 0){
      var = digitalRead(buttonPin); //BUTTONピンを読み取った結果をvarに代入
  
      //スイッチが押された瞬間を読み取る
      if(var == HIGH && old_var == LOW){
          buttonState = 1 - buttonState; //varがHIGHならstateの値が変わる
          delay(10); //チャタリング防止
          G_start_time = millis();   //タイマー開始時間を取得する
      }
      //スイッチが離された瞬間を読み取る
      if(var == LOW && old_var == HIGH){
          delay(10); //チャタリング防止
      }
  
      old_var = var; //varのをold_varに保存
  
      //stateは1か？
      if(buttonState == 1){
          pushed_count++;
      }
  }
}



void loop(){
//  Serial.println("start_time");
//  Serial.println(G_start_time);

//    digitalWrite(ledPin_1, HIGH);   //analogWrite(LED,brightness);で明るさを変えれる？
//    //https://github.com/houtbrion/arduino-grove/blob/master/rotary_angle_sensor/rotary_angle_sensor.ino
//    digitalWrite(ledPin_2, HIGH);

    unsigned long current_time;

    lcd.setCursor(6, 1);  //入力秒数は二桁まで

    int degrees;
    degrees = getDegree();  
    lcd.print(degrees);

//    digitalWrite(ledPin_1, HIGH);
//    digitalWrite(ledPin_2, HIGH);

    judge_switch();
//Serial.print("BS:");
//Serial.println(buttonState);
//Serial.print("  ");
  Serial.println(flag_decide_time);
    if (buttonState == 1 && flag_end == 0){
        if (flag_decide_time == 0){
          Serial.println("flag");
            G_time = degrees / 10;
            flag_decide_time++;
        }
        digitalWrite(ledPin_1, HIGH);
        digitalWrite(ledPin_2, HIGH);
        
        if(G_time > 0){  //変数がオーバーフローしてリセットされるのを防ぐ
//          Serial.println("lp");
//          Serial.println(G_time);
          current_time = millis();
//          Serial.println("current_time");
//          Serial.println(current_time);
          if(current_time - G_start_time >= ONE_SECOND) { //1秒ごとに
         //     Serial.println(G_start_time);
              G_time--;
              Serial.print("G_time:");
              Serial.println(G_time);
              if(G_time <= 9){
                  //10の位の表示を消して、1の位だけに表示
                  lcd.setCursor(6, 0);
                  lcd.print(clear_str);   
                  lcd.setCursor(7, 0);     
                  lcd.print(G_time);
                  if(G_time <= 0){  
                      flag_end++;
                      digitalWrite(ledPin_1, LOW);
                      digitalWrite(ledPin_2, LOW);
                      for(int i = 0; i < length; i++){
                          if(notes[i] == ' '){
                              delay(beats[i] * tempo); // rest
                          }else{
                              playNote(notes[i], beats[i] * tempo);
                          }            
                          // pause between notes
                          delay(tempo / 2);
                      }
                  }
              }else if(G_time <= 0){
                  lcd.print(G_time);
                  for(int i = 0; i < length; i++){
                      if(notes[i] == ' '){
                          delay(beats[i] * tempo); // rest
                      }else{
                          playNote(notes[i], beats[i] * tempo);
                      }            
                      // pause between notes
                      delay(tempo / 2);
                  }                
              }else{
                  lcd.print(G_time);
              }
              G_start_time = current_time;
          }
        }
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
