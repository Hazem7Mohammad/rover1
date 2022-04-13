/* This code for 6 wheel car project with mosfet H-bridge
 *  not L298N H-bridge
 *  The commented line are for L298N H-bridge
 */


int i=0;
// -------------- H-bridge connection pins -------- 
// Motor A
 
int enA = 11;//pwm pin for motor 2
int in1 = 12;// 1:Forward/ 0:Backward
//int in2 = 7;
 
// Motor B
 
int enB = 5;//pwm pin for motor 1
int in3 = 4;// 1:Forward/ 0:Backward
//int in4 = 4;

int speed = 120;// PWM from 0 to 255 

void setup() {

  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  //pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  //pinMode(in4, OUTPUT);

}

void loop() {


delay(3000);//count 2sec before starting

//
forward(); //left
delay(1000);
off();
delay(1000);
right();   // back 
delay(1000);
off();
delay(1000);
left();    // forward 
delay(1000);
off();
delay(1000);
backward();   // right 
delay(1000);
off();
delay(1000);

}


//-------------------H-bridge-------------------------------
// car movement functions: enabling required motors with appropriate speeds
void right(){
  analogWrite(enA, speed);
  digitalWrite(in1, LOW);
  //digitalWrite(in2, HIGH);
  analogWrite(enB, speed);
  digitalWrite(in3, LOW);
  //digitalWrite(in4, HIGH);
  
}
void left(){
  analogWrite(enB, speed);
  analogWrite(enA, speed);
  digitalWrite(in1, HIGH);
  //digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);
  //digitalWrite(in4, LOW); 
}
void backward(){
  
  analogWrite(enA, speed);
  digitalWrite(in1, HIGH);
  //digitalWrite(in2, LOW); 
  analogWrite(enB, speed); 
  digitalWrite(in3, LOW);
  //digitalWrite(in4, HIGH);
}
void forward(){
  
  analogWrite(enA, speed);
  digitalWrite(in1, LOW);
 // digitalWrite(in2, HIGH);
  analogWrite(enB, speed);  
  digitalWrite(in3, HIGH);
  //digitalWrite(in4, LOW);
}
void off(){
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(in1, LOW);
  //digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  //digitalWrite(in4, LOW);
}
