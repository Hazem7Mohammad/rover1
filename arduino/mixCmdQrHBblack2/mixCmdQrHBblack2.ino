//original code hbridge black 
#include <ros.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>

#include <ArduinoHardware.h>
#define pwm_2 5
#define pwm_1 11
#define dir_2 4
#define dir_1 12
#define servo 10

// flag 
std_msgs::Int16 flag;

// define give priorety for this exact pin 

int speed2 = 80;
int Speed = 40;//Change the speed of the car

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
ros::Publisher pub_flag( "flag", &flag);

ros::Subscriber<geometry_msgs::Pose2D> servo_n("pose", &callback );
/* create a subscriber node named servo_n that
* receive the msg (type Pose2D) from the topic
x_pose then invoke the callback
*/
//----------------------------------------------------


Servo Grip;

int pos = 0;




//Left and Right values
int lowval=240;//To turn right
int highval=400;//To turn left
float stoparea=40000;//After the area is above this number the car will stop       doesnt work 
int delayval=30;

//------------------Call Backs-----------------------------
void callback( const geometry_msgs::Pose2D& pose_message){
  nh.loginfo("Received a message");
if((y==0)&&(x==0)){

     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);
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

     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);

//         for (pos = 20; pos <= 127; pos += 1) { 
//      Grip.write(pos);              
//      delay(10);                       
//  }
Grip.write(120);
//flag.data=1;
delay(2000);
     nh.loginfo("Going back");
     analogWrite(pwm_2, Speed);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, Speed);
     digitalWrite(dir_1, HIGH);
     flag.data=1;
     pub_flag.publish(&flag);
delay(1000);

//flag = 1;
flag.data=0;

     
}

else if(lowval<x && x<highval &&  area > 44000){
     nh.loginfo("Going back");
     analogWrite(pwm_2, Speed);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, Speed);
     digitalWrite(dir_1, HIGH);
delay(delayval);
     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);
     
}



// if the target in the middle stop

else{ 
    nh.loginfo("Stop");

     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);
     Grip.write(20);
}
}
//---------------------------------------------------------


void setup()
{
 Motors_init();
 nh.initNode();
 nh.subscribe(sub);
  
// nh.initNode();
 nh.subscribe(servo_n);
 
 nh.advertise(pub_flag);

 flag.data=0;

//set the subscriber named servo_n to subscribe

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

    Grip.attach(servo);

Grip.write(20);
    

}
void loop()
{
 MotorL(w_l*10);
 MotorR(w_r*10);

////flag
//flag.data=0;
pub_flag.publish(&flag);

  
  nh.spinOnce();
//  delay(300);
}


void go_left (){
     analogWrite(pwm_2, Speed);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, Speed);
     digitalWrite(dir_1, LOW);
delay(delayval);
     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);
return ;
}

void go_right (){
     analogWrite(pwm_2, Speed);
     digitalWrite(dir_2, HIGH);
     analogWrite(pwm_1, Speed);
     digitalWrite(dir_1, HIGH);
delay(delayval);
     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);
     
return ;
}
void go_front (){
     analogWrite(pwm_2, Speed);
     digitalWrite(dir_2, HIGH);
     analogWrite(pwm_1, Speed);
     digitalWrite(dir_1, LOW);
delay(delayval);
     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);
     
return ;
}

void Motors_init(){
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


void MotorL(int Pulse_Width1){
 if (Pulse_Width1 > 0){
     nh.loginfo("Left wheel forward");
     analogWrite(pwm_1, Speed);
     digitalWrite(dir_1, LOW);
 }
 if (Pulse_Width1 < 0){
     nh.loginfo("Left wheel backward");
     analogWrite(pwm_1, Speed);
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
     analogWrite(pwm_2, Speed);
     digitalWrite(dir_2, HIGH);
 }
 if (Pulse_Width2 < 0){
     nh.loginfo("Right wheel backward");
     analogWrite(pwm_2, Speed);
     digitalWrite(dir_2, LOW);
 }
 if (Pulse_Width2 == 0){
     analogWrite(pwm_2, 0);
     digitalWrite(dir_2, HIGH);
 }
}
