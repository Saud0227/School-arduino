// This is a comment, a way to write text in code without the code runing the line

#include <LiquidCrystal.h>
// This imports code we need to run the LCD

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//This declairs variables as constants, meaning they wont change while the code is running
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//creates the lcd variable witch has the ports as inputs 

const int up = 7;
const int down = 6;
//more variables

void setup(){
    Serial.begin(9600);
    //opens a serial, a way to get feedback whilee runing code

    pinMode(up, INPUT);
    pinMode(down, INPUT);
    // selects the pins mode (Output writes high or low, input reads the pins state)

    // starts lcd
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

    //Test code for button

    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(millis() / 1000);
    //tst code for lcd
}