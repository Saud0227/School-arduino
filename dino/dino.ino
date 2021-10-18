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

byte dino[8] = {
	0b00110,
	0b01011,
	0b01110,
	0b01000,
	0b01110,
	0b01000,
	0b10110,
	0b10010
};
// Cactus
byte cactus[8] = {
	0b00100,
	0b10101,
	0b10101,
	0b11111,
	0b00100,
	0b00100,
	0b00100,
	0b00000
};

// Bird
byte bird[8] = {
	0b01000,
	0b11111,
	0b00110,
	0b00100,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};
int tmpT = 10;


void dispChar(byte charToWrite, int tx, int ty){
    lcd.setCursor(tx,ty);
    lcd.write(charToWrite);
}
void setup(){
    Serial.begin(9600);
    //opens a serial, a way to get feedback while runing code

    pinMode(up, INPUT);
    pinMode(down, INPUT);
    // selects the pins mode (Output writes high or low, input reads the pins state)

    // starts lcd
    lcd.begin(16, 2);
    lcd.print("hello, world!");

    lcd.createChar(0, dino); // create a new custom character
    lcd.createChar(1, cactus); // create a new custom character
    lcd.createChar(2, bird); // create a new custom character
}


void loop(){

    Serial.print(digitalRead(up));
    Serial.print("\nDown ");
    Serial.print(digitalRead(down));
    Serial.print("\n");
    /* Serial.print(tmpT);
    Serial.print("\n");

    if(digitalRead(up)==1){
        tmpT+=10;
    }
    if(digitalRead(down)==1){
        tmpT-=10;
    } */

    //Test code for button

    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(millis()/1000);
    //tst code for lcd and buttons

    /* lcd.setCursor(7,1);
    lcd.print(tmpT);
    if(tmpT <= 0){
        delay(1);
    }else{
        delay(tmpT);
    }
     lcd.setCursor(7,1);
    lcd.write((byte)0); */
    dispChar(((byte)0),7,1);
    delay(25);
}