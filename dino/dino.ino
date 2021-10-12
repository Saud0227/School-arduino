const int up = 7;
const int down = 6;


void setup(){
    Serial.begin(9600);
    pinMode(up, INPUT);
    pinMode(down, INPUT);
}

void loop(){;
    Serial.print(analogRead(up),analogRead(down));
    delay(250);
}