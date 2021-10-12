#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int up = 7;
const int down = 6;


void setup(){
    Serial.begin(9600);
    pinMode(up, INPUT);
    pinMode(down, INPUT);

    lcd.begin(16, 2);
    lcd.print("hello, world!");
}


void loop(){
    /* Serial.print("up ");
    Serial.print(digitalRead(up));
    Serial.print("\nDown ");
    Serial.print(digitalRead(down));
    Serial.print("\n");
    delay(250); */

    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(millis() / 1000);
}