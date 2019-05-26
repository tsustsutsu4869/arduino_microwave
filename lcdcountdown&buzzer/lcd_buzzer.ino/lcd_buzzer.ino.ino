
#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"
//#include "pitches.h"


Servo Motor;             // Motorという名前でプログラム内で使いますという宣言
int buttonPin = 2;       // ボタンのコネクタ番号（D2につないだ場合）
int ledPin_1 = 4;       // LEDのコネクタ番号（D4につないだ場合）
int ledPin_2 = 8;       // LEDのコネクタ番号（D4につないだ場合）
int buzzerPin = 6;        // ブザーのコネクタ番号（D6につないだ場合）
//int motorPin = 8;        // モータのコネクタ番号（D8につないだ場合）
int buttonState;         // ボタンの状態
int shaft = 90;          // モータの角度を記憶する変数（90度から開始）


rgb_lcd lcd;

const int colorR = 255;
const int colorG = 45;
const int colorB = 0;

//タイマーの値
const int TIME_VALUE = 12;
//1分間（ミリ秒）
const unsigned long ONE_SECOND = 1000L;  //1秒
//-----------------------
// グローバル変数の定義
//-----------------------
unsigned long G_time = TIME_VALUE;                   //タイマーの時間
unsigned long G_start_time;                          //スタートボタンを押してからの時間

char clear_str=' ';
int flag_end=0;

int length = 12; // the number of notes
char notes[] = "gfg gfg gfg "; // a space represents a rest
long beats[] = { 1.2, 1.2, 1.2, 1, 1.2, 1.2, 1.2, 1, 1.2, 1.2, 1.2, 1};
int tempo = 150;

void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);

    //開始時間を取得する
    G_start_time = millis();
    // Print a message to the LCD.
    lcd.setCursor(9, 0);
    lcd.print("seconds");

    pinMode(buzzerPin, OUTPUT);  // ブザーをOUTPUTとして設定
    pinMode(ledPin_1, OUTPUT);  // ブザーをOUTPUTとして設定
    pinMode(ledPin_2, OUTPUT);  // ブザーをOUTPUTとして設定
    

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


void loop() 
{
    unsigned long current_time;

    if (flag_end == 0){
      digitalWrite(ledPin_1, HIGH);
      digitalWrite(ledPin_2, HIGH); 
    }

    lcd.setCursor(6, 0);
    
    if(G_time > 0){  //変数がオーバーフローしてリセットされるのを防ぐ
        current_time = millis();
        if(current_time - G_start_time >= ONE_SECOND) { //1秒ごとに
            Serial.println(G_start_time);
            G_time--;
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
                lcd.setCursor(2, 1);     
                lcd.print("tt");
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

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
