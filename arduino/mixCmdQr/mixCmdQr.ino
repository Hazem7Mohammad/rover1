//original code hbridge red 
#include <ros.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>

#include <ArduinoHardware.h>
#define EN_L 3
#define IN1_L 8
#define IN2_L 7
#define EN_R 6
#define IN1_R 5
#define IN2_R 4

int speed2 = 80;
int turnSpeed = 200;

#include <Servo.h>

double w_r=0, w_l=0;
//wheel_rad is the wheel radius ,wheel_sep is
double wheel_rad = 0.0325, wheel_sep = 0.295;
ros::NodeHandle nh;
int lowSpeed = 200;
int highSpeed = 50;
float x = 0.0;
float y = 0.0;
float area =0.0;
double speed_ang=0, speed_lin=0;
void messageCb( const geometry_msgs::Twist& msg){
  speed_ang = msg.angular.z;
  speed_lin = msg.linear.x;
  w_r = (speed_lin/wheel_rad) + ((speed_ang*wheel_sep)/(2.0*wheel_rad));
  w_l = (speed_lin/wheel_rad) - ((speed_ang*wheel_sep)/(2.0*wheel_rad));
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );
void Motors_init();
void MotorL(int Pulse_Width1);
void MotorR(int Pulse_Width2);


//ros::NodeHandle
//nh;
//float x = 0.0;
//float y = 0.0;
//float area =0.0;

Servo Grip;

int pos = 0;


int speed = 90;//Change the speed of the car


//Left and Right values
int lowval=240;//To turn right
int highval=400;//To turn left
float stoparea=40000;//After the area is above this number the car will stop       doesnt work 
int delayval=30;

//------------------Call Backs-----------------------------
void callback( const geometry_msgs::Pose2D& pose_message){
  nh.loginfo("Received a message");
if((y==0)&&(x==0)){
     analogWrite(EN_R, speed);
     digitalWrite(IN1_R, LOW);
     digitalWrite(IN2_R, LOW);
     analogWrite(EN_L, speed);
     digitalWrite(IN1_L, LOW);
     digitalWrite(IN2_L, LOW);
  // saving the messages in x & y variables
  
}
  x = pose_message.x;
  y = pose_message.y;
  area = pose_message.theta;
  
  //
if(lowval<x && x<highval && area < stoparea){
     nh.loginfo("Going forward");
     go_front();
     Grip.write(20);

     
}
//Target on the right
else if(x<lowval){
  nh.loginfo("Going right");
go_right();
Grip.write(20);
}
//target on the left
else if(x>highval){
  nh.loginfo("Going left");
go_left();
Grip.write(20);

}

else if(area > stoparea && area < 44000 && lowval<x && x<highval){
      nh.loginfo("Stop to Catch");
     analogWrite(EN_R, speed);
     digitalWrite(IN1_R, LOW);
     digitalWrite(IN2_R, LOW);
     analogWrite(EN_L, speed);
     digitalWrite(IN1_L, LOW);
     digitalWrite(IN2_L, LOW);

//         for (pos = 20; pos <= 127; pos += 1) { 
//      Grip.write(pos);              
//      delay(10);                       
//  }
Grip.write(120);

     
}

else if(lowval<x && x<highval &&  area > 44000){
     nh.loginfo("Going back");
//     go_front();
     analogWrite(EN_L, speed2);
     digitalWrite(IN1_L, HIGH);
     digitalWrite(IN2_L, LOW);
     analogWrite(EN_R, speed2);
     digitalWrite(IN1_R, LOW);
     digitalWrite(IN2_R, HIGH);
delay(20);
    digitalWrite(EN_L, LOW);
    digitalWrite(EN_R, LOW);
    digitalWrite(IN1_L, LOW);
    digitalWrite(IN2_L, LOW);
    digitalWrite(IN1_R, LOW);
    digitalWrite(IN2_R, LOW);
     
}



// if the target in the middle stop

else{ 
    nh.loginfo("Stop");
     analogWrite(EN_R, speed);
     digitalWrite(IN1_R, LOW);
     digitalWrite(IN2_R, LOW);
     analogWrite(EN_L, speed);
     digitalWrite(IN1_L, LOW);
     digitalWrite(IN2_L, LOW);
     Grip.write(20);
}
}
//---------------------------------------------------------
ros::Subscriber<geometry_msgs::Pose2D> servo_n("pose", &callback );
/* create a subscriber node named servo_n that
* receive the msg (type Pose2D) from the topic
x_pose then invoke the callback
*/
//----------------------------------------------------
void setup()
{
 Motors_init();
// nh.initNode();
 nh.subscribe(sub);
  
 nh.initNode();
 nh.subscribe(servo_n);
//set the subscriber named servo_n to subscribe

  Serial.begin(57600);
  pinMode(EN_L, OUTPUT);
  pinMode(EN_R, OUTPUT);
  pinMode(IN1_L, OUTPUT);
  pinMode(IN2_L, OUTPUT);
  pinMode(IN1_R, OUTPUT);
  pinMode(IN2_R, OUTPUT);
  digitalWrite(EN_L, LOW);
  digitalWrite(EN_R, LOW);
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);

    Grip.attach(11);

Grip.write(20);
    

}
void loop()
{
 MotorL(w_l*10);
 MotorR(w_r*10);
// nh.spinOnce();
  
  nh.spinOnce();
//  delay(300);
}


void go_left (){
     analogWrite(EN_L, speed);
     digitalWrite(IN1_L, HIGH);
     digitalWrite(IN2_L, LOW);
     analogWrite(EN_R, speed);
     digitalWrite(IN1_R, HIGH);
     digitalWrite(IN2_R, LOW);
     delay(delayval);
    digitalWrite(EN_L, LOW);
    digitalWrite(EN_R, LOW);
    digitalWrite(IN1_L, LOW);
    digitalWrite(IN2_L, LOW);
    digitalWrite(IN1_R, LOW);
    digitalWrite(IN2_R, LOW);
return ;
}

void go_right (){
     analogWrite(EN_L, speed);
     digitalWrite(IN1_L, LOW);
     digitalWrite(IN2_L, HIGH);
     analogWrite(EN_R, speed);
     digitalWrite(IN1_R, LOW);
     digitalWrite(IN2_R, HIGH);
delay(delayval);
    digitalWrite(EN_L, LOW);
    digitalWrite(EN_R, LOW);
    digitalWrite(IN1_L, LOW);
    digitalWrite(IN2_L, LOW);
    digitalWrite(IN1_R, LOW);
    digitalWrite(IN2_R, LOW);
     
return ;
}
void go_front (){
     analogWrite(EN_L, speed2);
     digitalWrite(IN1_L, LOW);
     digitalWrite(IN2_L, HIGH);
     analogWrite(EN_R, speed2);
     digitalWrite(IN1_R, HIGH);
     digitalWrite(IN2_R, LOW);
delay(delayval);
    digitalWrite(EN_L, LOW);
    digitalWrite(EN_R, LOW);
    digitalWrite(IN1_L, LOW);
    digitalWrite(IN2_L, LOW);
    digitalWrite(IN1_R, LOW);
    digitalWrite(IN2_R, LOW);
     
return ;
}

void Motors_init(){
 pinMode(EN_L, OUTPUT);
 pinMode(EN_R, OUTPUT);
 pinMode(IN1_L, OUTPUT);
 pinMode(IN2_L, OUTPUT);
 pinMode(IN1_R, OUTPUT);
 pinMode(IN2_R, OUTPUT);
 digitalWrite(EN_L, LOW);
 digitalWrite(EN_R, LOW);
 digitalWrite(IN1_L, LOW);
 digitalWrite(IN2_L, LOW);
 digitalWrite(IN1_R, LOW);
 digitalWrite(IN2_R, LOW);
}


void MotorL(int Pulse_Width1){
 if (Pulse_Width1 > 0){
     nh.loginfo("Left wheel forward");
     analogWrite(EN_L, speed2);
     digitalWrite(IN1_L, LOW);
     digitalWrite(IN2_L, HIGH);
 }
 if (Pulse_Width1 < 0){
     nh.loginfo("Left wheel backward");
     Pulse_Width1=abs(Pulse_Width1);
     analogWrite(EN_L, speed2);
     digitalWrite(IN1_L, HIGH);
     digitalWrite(IN2_L, LOW);
 }
 if (Pulse_Width1 == 0){
     analogWrite(EN_L, speed2);
     digitalWrite(IN1_L, LOW);
     digitalWrite(IN2_L, LOW);
 }
}


void MotorR(int Pulse_Width2){
 if (Pulse_Width2 > 0){
     nh.loginfo("Right wheel forward");
     analogWrite(EN_R, speed2);
     digitalWrite(IN1_R, HIGH);
     digitalWrite(IN2_R, LOW);
 }
 if (Pulse_Width2 < 0){
     nh.loginfo("Right wheel backward");
     Pulse_Width2=abs(Pulse_Width2);
     analogWrite(EN_R, speed2);
     digitalWrite(IN1_R, LOW);
     digitalWrite(IN2_R, HIGH);
 }
 if (Pulse_Width2 == 0){
     analogWrite(EN_R, speed2);
     digitalWrite(IN1_R, LOW);
     digitalWrite(IN2_R, LOW);
 }
}
