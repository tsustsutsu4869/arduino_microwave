
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
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

    delay(1000);
}

void loop() 
{
    unsigned long current_time;

    lcd.setCursor(6, 0);

    if(G_time > 0) {
        current_time = millis();
        if(current_time - G_start_time >= ONE_SECOND) { //1秒ごとに
            G_time--;
            if(G_time <= 9) {
                //10の位の表示を消して、1の位だけに表示
                lcd.setCursor(6, 0);
                lcd.print(clear_str);   
                lcd.setCursor(7, 0);     
                lcd.print(G_time);
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
