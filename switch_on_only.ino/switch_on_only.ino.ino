//ボタンを押すとスイッチONOFFを切り替えられるプログラム

const int LED = 8; //定数LEDを設定
const int BUTTON = 2; //定数BUTTONを設定

int var = 0; //変数varを設定
int old_var = 0; //変数old_varを設定
int state = 0; //変数stateを設定
int pushed_count=0;

void setup() {
  pinMode(LED, OUTPUT); //LEDピンを出力に設定
  pinMode(BUTTON, INPUT); //BUTTONピンを入力に設定
}

void loop() {
  if (pushed_count == 0){
      var = digitalRead(BUTTON); //BUTTONピンを読み取った結果をvarに代入
  
      //スイッチが押された瞬間を読み取る
      if(var == HIGH && old_var == LOW){
          state = 1 - state; //varがHIGHならstateの値が変わる
          delay(10); //チャタリング防止
      }
      //スイッチが離された瞬間を読み取る
      if(var == LOW && old_var == HIGH){
          delay(10); //チャタリング防止
      }
  
      old_var = var; //varのをold_varに保存
  
      //stateは1か？
      if(state == 1){
          digitalWrite(LED, HIGH); //LEDピンにHIGHを出力
          pushed_count++;
      }
      else{
          digitalWrite(LED, LOW); //LEDピンにLOWを出力
      }
  }
}
