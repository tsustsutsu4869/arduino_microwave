/*
  サンプル1
  - ポタンを押している間だけモータが90度回転します
*/
#include <Servo.h>  // サーボモータを使うときのおまじない

/* ここで，プログラム内で使う変数を宣言します */
Servo Motor;             // Motorという名前でプログラム内で使いますという宣言
int buttonPin = 7;       // ボタンのコネクタ番号（D7につないだ場合）
int motorPin = 5;        // モータのコネクタ番号（D5につないだ場合）
int buttonState;         // ボタンの状態
int shaft = 90;          // モータの角度を記憶する変数（90度から開始）

/*
  setupは最初に1度だけ行うセンサやアクチュエータの設定
  各コネクタがINPUT（センサ）かOUTPUT（アクチュエータ）かをここで指定します
*/
void setup() {
  Motor.attach(motorPin);     // サーボモータの接続
  Motor.write(shaft);         // サーボモータを90度の位置に設定（0度だと微振動するかもしれないので…）
  pinMode(buttonPin, INPUT);  // ポタンをINPUTとして設定
}

/*
  loopの中に動かしたいプログラムを書きます
  loopの中の処理はずっと繰り返し行われます（delayを入れなければ1瞬で1周を終えます）
*/
void loop() {

  // ボタンの状態を読み込む（押している間はHIGH）
  buttonState = digitalRead(buttonPin);  

  // ボタンが押されているか否かで角度を変更
  if (buttonState == HIGH) { 
    shaft = 180;
  } else {
    shaft = 90;
  }

  // shaftで指定された位置に回転
  Motor.write(shaft);
  
  // スイッチのチャタリングの影響を軽減する
  delay(15);
}

