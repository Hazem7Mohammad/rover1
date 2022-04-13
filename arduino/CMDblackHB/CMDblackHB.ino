//CMD_VEL topic for teleop using MDD10A H-Bridge
// black hbridge 
#include <ArduinoHardware.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#define pwm_2 5
#define pwm_1 11
#define dir_2 4
#define dir_1 12
int speed = 80;
int turnSpeed = 200;
double w_r=0, w_l=0;
//wheel_rad is the wheel radius ,wheel_sep is
double wheel_rad = 0.0325, wheel_sep = 0.295;
ros::NodeHandle nh;
int lowSpeed = 200;
int highSpeed = 50;
double speed_ang=0, speed_lin=0;
void messageCb( const geometry_msgs::Twist& msg){
  speed_ang = msg.angular.z;
  speed_lin = msg.linear.x;
  w_r = (speed_lin/wheel_rad) + ((speed_ang*wheel_sep)/(2.0*wheel_rad));
  w_l = (speed_lin/wheel_rad) - ((speed_ang*wheel_sep)/(2.0*wheel_rad));
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );
void MotorL(int Pulse_Width1);
void MotorR(int Pulse_Width2);
void setup(){
 nh.initNode();
 nh.subscribe(sub);
//}
//void setup() {
pinMode(pwm_2, OUTPUT);
pinMode(pwm_1, OUTPUT);
pinMode(dir_1, OUTPUT);
pinMode(dir_2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(6, OUTPUT);
analogWrite(3, LOW);
analogWrite(6, LOW);

analogWrite(pwm_1, LOW);
analogWrite(pwm_2, LOW);



Serial.begin(57600);
}
void loop(){
 MotorL(w_l*10);
 MotorR(w_r*10);
 nh.spinOnce();
}




void MotorL(int Pulse_Width1){
 if (Pulse_Width1 > 0){
     nh.loginfo("Left wheel forward");
     analogWrite(pwm_1, speed);
     digitalWrite(dir_1, LOW);
 }
 if (Pulse_Width1 < 0){
     nh.loginfo("Left wheel backward");
     analogWrite(pwm_1, speed);
     digitalWrite(dir_1, HIGH);
 }
 if (Pulse_Width1 == 0){
     analogWrite(pwm_1, 0);
     digitalWrite(dir_1, HIGH);
 }
}


void MotorR(int Pulse_Width2){
 if (Pulse_Width2 > 0){
     nh.loginfo("Right wheel forward");
     analogWrite(pwm_2, speed);
     digitalWrite(dir_2, HIGH);
 }
 if (Pulse_Width2 < 0){
     nh.loginfo("Right wheel backward");
     analogWrite(pwm_2, speed);
     digitalWrite(dir_2, LOW);
 }
 if (Pulse_Width2 == 0){
     analogWrite(pwm_2, 0);
     digitalWrite(dir_2, HIGH);
 }
}
