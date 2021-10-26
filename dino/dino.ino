// This is a comment, a way to write text in code without the code runing the line
#pragma region

#include <LiquidCrystal.h>
// This imports code we need to run the LCD

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//This declairs variables as constants, meaning they wont change while the code is running
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//creates the lcd variable witch has the ports as inputs 

#pragma endregion

const int up = 7;
const int down = 6;

const int tickS = 10;
bool gameOver = true;

int jumpRaw;
int crouchButton;

const int jumpDuration = 0.5*1000/tickS;
//more variables

// Game vars

int jumpT  = -1;
bool crouchState = false;
bool jumpstate = false;
int blockXPos[10];
int blockType[10];

int sqToClearX[10];
int sqToClearY[10];

int moveT = -1;
const int moveTr = 15;

int spawnT = -1;
int spawnTr = 200;
int mutIncr = 0;

int countReduct = 0;
bool starup = true;

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

byte Crouch1[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b01111,
	0b10101,
	0b10100
};

byte Crouch2[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b01111,
	0b10101,
	0b10100
};

void setup(){
    Serial.begin(9600);
    //opens a serial, a way to get feedback while runing code

    pinMode(up, INPUT);
    pinMode(down, INPUT);
    // selects the pins mode (Output writes high or low, input reads the pins state)

    // starts lcd
    lcd.begin(16, 2);
    // lcd.print("hello, world!");

    lcd.createChar(0, dino); // create a new custom character
    lcd.createChar(1, cactus); // create a new custom character
    lcd.createChar(2, bird); // create a new custom character
    lcd.createChar(3, Crouch1); // Left part of crouch Frame
    lcd.createChar(4, Crouch2); // Right part of crouch Frame

	//-----------------------------------------

	for(int i = 0; i < sizeof(blockXPos)/sizeof(blockXPos[0]); i++){
		// Serial.print(i + "\n");
		blockXPos[i] = -1;
		blockType[i] = -1;
	}

	blockXPos[0] = 20;
	blockType[0] = 1;
	/* blockXPos[1] = 8;
	blockType[1] = 2;
	blockXPos[2] = 9;
	blockType[2] = 0; */

}


//--------------------------------------------------

void dispChar(byte charToWrite, int tx, int ty){

    lcd.setCursor(tx,ty);
    lcd.write(charToWrite);
}
void clearChar(int tx, int ty){
	lcd.setCursor(tx,ty);
    lcd.write(" ");
}

void storeToClear(int tx, int ty){
	for (int i = 0; i < sizeof(sqToClearX)/sizeof(sqToClearX[0]); i++){
		if(sqToClearX[i] == -1){
			sqToClearX[i] = tx;
			sqToClearY[i] = ty;
			break;
		}
	}

}

bool checkMoveBlocker(){
	return(moveT > moveTr);
}

void tickMoveBlocker(){
	if(moveT > moveTr){
		moveT = -1;
	}else{
		moveT++;
	}

}


void spawnB(){
	if(spawnT>spawnTr){
		spawnT = 0;
		int blockTypeGen = rand() % 100;
		int toSet = 2;
		if(blockTypeGen<66){
			toSet = 0;

		}else if (blockTypeGen<80){
			toSet = 1;

		}
		int ranOfset = rand() %4;
		ranOfset -= 2;
		bSpawn(20 + ranOfset, toSet);
		if(rand() % 100 > 66){
			bSpawn(20 + rand() % 5, toSet);
		}

	}else{
		spawnT+=1;
	}
}

void bSpawn(int x, int type){
	for(int i = 0; i < sizeof(blockXPos)/sizeof(blockXPos[0]); i++){
		if(blockXPos[i]==-1){
			blockXPos[i] = x;
			blockType[i] = type;
			Serial.print("Blocker @:" + String(i));
			Serial.print("\n");
			break;
		}
	}
}
//--------------------------------------------------

void loop(){
	// lcd.clear();
	if(gameOver){
		jumpRaw = digitalRead(up);
		if(starup){
			lcd.setCursor(6,0);
			lcd.print("Start");
		}else{
			lcd.setCursor(3,0);
			lcd.print("Gamer Over");
		}
		if(jumpRaw == HIGH){
			gameOver = false;
			if(starup){
				starup = false;
			}
			srand(millis());
			countReduct = millis();
			for(int i = 0; i < sizeof(blockXPos)/sizeof(blockXPos[0]); i++){
				// Serial.print(i + "\n");
				blockXPos[i] = -1;
				blockType[i] = -1;
			}
			for(int i = 0; i < 16; i++){
				clearChar(i,0);
			}
		}

	}else{

	    jumpRaw = digitalRead(up);
	    crouchButton = digitalRead(down);



	    if (jumpRaw == HIGH && jumpT < 0) {
	        jumpstate = true;
			Serial.print("Jump!\n");
	        jumpT = jumpDuration;
			clearChar(2,1);
	    }else if (jumpT > -1){
			jumpT-=1;
		}else if(jumpT < 0 && jumpstate == true){
			jumpstate = false;
			clearChar(2,0);
		}

	    if (crouchButton == HIGH && crouchState != true) {
			crouchState = true;
			Serial.print("crouch\n");
	        if(jumpstate){
				jumpstate = false;
				jumpT = -1;
				clearChar(2,0);
			}
	    }else if(crouchState && crouchButton == LOW)
		{
			crouchState = false;
			Serial.print("Stands\n");
			clearChar(3,1);
		}


	    lcd.setCursor(14,0);

	    // print the number of seconds since reset:
	    lcd.print((millis()-countReduct)/1000);

		if(checkMoveBlocker()){
			for (int i = 0; i < sizeof(sqToClearX)/sizeof(sqToClearX[0]); i++){
				if(sqToClearX[i] != -1){
					clearChar(sqToClearX[i],sqToClearY[i]);
					sqToClearX[i] = -1;
					sqToClearY[i] = -1;
				}
			}
		}




		if(checkMoveBlocker()){
			for(int i = 0; i < sizeof(blockXPos)/sizeof(blockXPos[0]); i++){
				if(blockXPos[i]>-1){
					if(blockXPos[i] == 2){

						//Checking col
						//-------------------------------------------------------
						if(blockType[i] == 0 && jumpstate==false){
							gameOver = true;
						}else if (blockType[i] == 1 && jumpstate)
						{
							gameOver = true;
						}else if (blockType[i] == 2 && crouchState == false){
							gameOver = true;
						}
						//-------------------------------------------------------

					}
					if(blockXPos[i]<16 && gameOver == false){


						//-------------------------------------------------------
						if(blockType[i] == 2){
							dispChar((byte)2,blockXPos[i],1);
							storeToClear(blockXPos[i],1);
						}else if (blockType[i]==1){
							dispChar((byte)2,blockXPos[i],0);
							storeToClear(blockXPos[i],0);
						}else if (blockType[i]==0){
							dispChar((byte)1,blockXPos[i],1);
							storeToClear(blockXPos[i],1);
						}
						//-------------------------------------------------------
					}
					blockXPos[i] -=1;
				}
			}
		}




		/* Serial.print(jumpstate);
		Serial.print(crouchState);
		Serial.print("\n"); */


		if(jumpstate){
			dispChar((byte)0,2,0);
		}else if (crouchState){
			dispChar((byte)3,2,1);
			dispChar((byte)4,3,1);
		}else{
			dispChar((byte)0,2,1);
		}
	/*
	Blocker key:
	0 => Cactus
	1 => High bird
	2 => Low bird
	*/
	}
	delay(tickS);
	tickMoveBlocker();
	spawnB();
}