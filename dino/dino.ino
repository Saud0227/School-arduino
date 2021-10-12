const int up = 7;
const int down = 6;


void setup(){
    Serial.begin(9600);
    pinMode(up, INPUT);
    pinMode(down, INPUT);
}

void loop(){
    Serial.print("up ");
    Serial.print(digitalRead(up));
    Serial.print("\nDown ");
    Serial.print(digitalRead(down));
    Serial.print("\n");
    delay(250);
}