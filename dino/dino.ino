

// This is a comment, a way to write text in code without the code runing the line
#pragma region

#include <LiquidCrystal.h>
// This imports code we need to run the LCD

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//This declairs variables as constants, meaning they wont change while the code is running
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//creates the lcd variable witch has the ports as inputs 

#pragma endregion

// Select pins for up and down
const int up = 7;
const int down = 6;

// Tcik speed variable for movement, gameover
const int tickS = 10;
bool gameOver = true;

// Variables for storing button input
int jumpRaw;
int crouchButton;

// Jump duration to tweek balance if needed
const int jumpDuration = 0.5*1000/tickS;



// Game vars

int jumpT  = -1;
bool crouchState = false;
bool jumpstate = false;

//Arrays storing blockers info
int blockXPos[10];
int blockType[10];

int sqToClearX[10];
int sqToClearY[10];

//Update clock
int moveT = -1;
const int moveTr = 15;

int spawnT = -1;
int spawnTr = 200;

//time var
int countReduct = 0;

//if first time playing
bool starup = true;


// custum chars

//dino (ground normal)
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


}


//--------------------------------------------------
// dispchar function
void dispChar(byte charToWrite, int tx, int ty){

    lcd.setCursor(tx,ty);
    lcd.write(charToWrite);
}
// clear a square on the grid
void clearChar(int tx, int ty){
	lcd.setCursor(tx,ty);
    lcd.write(" ");
}
// add square to clear next tick (to not leave ghost blockers)
void storeToClear(int tx, int ty){
	for (int i = 0; i < sizeof(sqToClearX)/sizeof(sqToClearX[0]); i++){
		if(sqToClearX[i] == -1){
			sqToClearX[i] = tx;
			sqToClearY[i] = ty;
			break;
		}
	}

}
// simple return check
bool checkMoveBlocker(){
	return(moveT > moveTr);
}
// clock move tick
void tickMoveBlocker(){
	if(moveT > moveTr){
		moveT = -1;
	}else{
		moveT++;
	}

}

// spawn blocker logic
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
// actuall spwaning of blocker
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
//Main loop
void loop(){
	// lcd.clear();
	if(gameOver){
		//Run game over code
		jumpRaw = digitalRead(up);
		if(starup){
			lcd.setCursor(6,0);
			lcd.print("Start");
		}else{
			lcd.setCursor(3,0);
			lcd.print("Gamer Over");
		}
		if(jumpRaw == HIGH){
			//start
			gameOver = false;
			if(starup){
				starup = false;
			}
			//seed for random generated from the milli second the game starts / restarts
			srand(millis());
			//for acurate timer
			countReduct = millis();
			
			//clear all blockers
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
		// if game is live
	    jumpRaw = digitalRead(up);
	    crouchButton = digitalRead(down);


		//if on ground and trying to jump
	    if (jumpRaw == HIGH && jumpT < 0) {
	        jumpstate = true;
			Serial.print("Jump!\n");
	        jumpT = jumpDuration;
			// removeing ground dino texture
			clearChar(2,1);

	    }else if (jumpT > -1){
			//if in air, tick air time
			jumpT-=1;
		}else if(jumpT < 0 && jumpstate == true){
			//if jump is done reset unless you want to jump again
			jumpstate = false;
			clearChar(2,0);
		}

	    if (crouchButton == HIGH && crouchState != true) {
			//activate crouch if not allready active when pressing button
			crouchState = true;
			Serial.print("crouch\n");
	        if(jumpstate){
				//if jumping when trying to crouch, land
				jumpstate = false;
				jumpT = -1;
				clearChar(2,0);
			}
	    }else if(crouchState && crouchButton == LOW){
			//if crouch state is true butt button is not pressed
			crouchState = false;
			Serial.print("Stands\n");
			clearChar(3,1);
		}

	    // print the number of seconds since reset:
	    lcd.setCursor(14,0);
	    lcd.print((millis()-countReduct)/1000);

		//move blockers if they shoudl be moved
		if(checkMoveBlocker()){
			//clear ghost blocks
			for (int i = 0; i < sizeof(sqToClearX)/sizeof(sqToClearX[0]); i++){
				if(sqToClearX[i] != -1){
					clearChar(sqToClearX[i],sqToClearY[i]);
					sqToClearX[i] = -1;
					sqToClearY[i] = -1;
				}
			}
		




			//for each blocker....
			for(int i = 0; i < sizeof(blockXPos)/sizeof(blockXPos[0]); i++){
				if(blockXPos[i]>-1){
					if(blockXPos[i] == 2){

						//.. checking colission
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

						//draw if they should be drawn
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
					//actually do the moving
					blockXPos[i] -=1;
				}
			}
		}




		/* Serial.print(jumpstate);
		Serial.print(crouchState);
		Serial.print("\n"); */

		//and finally
		//draw the player
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
	//do final ticks and runtime delay
	delay(tickS);
	tickMoveBlocker();
	spawnB();
}