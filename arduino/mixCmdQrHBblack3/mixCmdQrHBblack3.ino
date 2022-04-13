//original code hbridge black 
#include <ros.h>
#include <std_msgs/Int16.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>
#include <Servo.h>
#include <ArduinoHardware.h>

//---------------------- Define arduino pins-----------------------
/* define: makes the compiler replace any presence of "pwm_2" to "5" */
#define pwm_2 5 
#define pwm_1 11
#define dir_2 4
#define dir_1 12
#define servo 10

 


//--------------------- Variables-----------------------

Servo Grip; // Set servo variable name as "Grip"

//<<<<<<< Car control:>>>>>>
int Speed = 40;//Change the speed of the car
double w_r=0, w_l=0;
double wheel_rad = 0.0325; //wheel radius
double wheel_sep = 0.295; //**************
double speed_ang=0, speed_lin=0;


// <<<<<<< QR Code:>>>>>>>
float x = 0.0; // x & y are the QR center
float y = 0.0;
float area = 0.0; //QR area indicate how close the QR to the camera
/* The camera screen(480x640) devided into 3 vertical regions:
1) left region: bounded from 0(start edge) to "lowval"
2) Right region: bounded from "highval" to 640(end edge)
3) Middle region: bounded from "lowval" to "highval"
The idea is to keep robot moving till the QR code in the middle */
int lowval = 240; // left region end boundary 
int highval = 400; // right region start boundary
float stoparea = 40000; //Threshold area where the car should stop after
int delayval = 30; // Variable used for the delay value



//????????????????????????????????
void messageCb( const geometry_msgs::Twist& msg){
  speed_ang = msg.angular.z;
  speed_lin = msg.linear.x;
  w_r = (speed_lin/wheel_rad) + ((speed_ang*wheel_sep)/(2.0*wheel_rad));
  w_l = (speed_lin/wheel_rad) - ((speed_ang*wheel_sep)/(2.0*wheel_rad));
}


//??????????????????????
void Motors_init();
void MotorL(int Pulse_Width1);
void MotorR(int Pulse_Width2);
//????????????????????????

std_msgs::Int16 Flag; // define "Flag" message with type int
ros::NodeHandle nh; // Set node handler name as "nh"

//--------------------Pusblishers and Subscribers--------------
ros::Publisher pub_flag( "flag", &Flag);
/* create a publisher named "pub_flag" that publish the msg "Flag"
 (type int) to the topic "flag" */
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );
/* create a subscriber node named "sub" that receive the msg 
(type Twist) from the topic "cmd_vel" then invoke  "messageCB" */
ros::Subscriber<geometry_msgs::Pose2D> servo_n("pose", &callback );
/* create a subscriber node named "servo_n" that receive the msg 
(type Pose2D) from the topic "pose" then invoke the callback */
//----------------------------------------------------




//------------------QR code Call Back-----------------------------
void callback( const geometry_msgs::Pose2D& pose_message){
     // saving the messages in x, y & area variables
     nh.loginfo("Received a message");//print on terminal window
     x = pose_message.x;
     y = pose_message.y;
     area = pose_message.theta;

     // check when there is no QR, is it sending zeros????????????????
     if((y==0)&&(x==0)){
     analogWrite(pwm_2, LOW);
     digitalWrite(dir_2, LOW);
     analogWrite(pwm_1, LOW);
     digitalWrite(dir_1, LOW);
     }

  // If the QR in the middle but far (need to move forward)
     if(lowval<x && x<highval && area < stoparea){
          nh.loginfo("Going forward");//print on terminal window
          go_front();
//          Grip.write(20); //*****************    
     }
     // If the QR in the middle but too close (need to move backward)
     else if(lowval<x && x<highval &&  area > 44000){
          nh.loginfo("Going back");//print on terminal window
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
     // If the QR in left region (need to go right)
     else if(x<lowval){
          nh.loginfo("Going right");//print on terminal window
          go_right();
//          Grip.write(20);//***********
     }
     //If the QR in the right region (need to go left)
     else if(x>highval){
          nh.loginfo("Going left");//print on terminal window
          go_left();
//          Grip.write(20);//*************
     }
     // If the QR in the Gripper range
     else if(area > stoparea && area < 44000 && lowval<x && x<highval){
          // Stop the robot
          analogWrite(pwm_2, LOW);
          digitalWrite(dir_2, LOW);
          analogWrite(pwm_1, LOW);
          digitalWrite(dir_1, LOW);
          //Check to Catch or to Drop
          switch (Flag.data){
               case 0: // If the Flag is 0 >> Catch
                    nh.loginfo("Stop to Catch");//print on terminal window
                    Grip.write(120);// Close the gripper
                    Flag.data = 1;//Change flag to turn off the QR python code
                    break; // Exit the switch statement 
               case 1: // If the Flag is 1 >> Drop
                    nh.loginfo("Stop to Drop");//print on terminal window
                    Grip.write(20);// Open the gripper
                    Flag.data = 2; //Change flag to turn off the QR python code
                    break; // Exit the switch statement 
          }
          delay(2000);
          nh.loginfo("Going back");//print on terminal window    
          analogWrite(pwm_2, Speed);
          digitalWrite(dir_2, LOW);
          analogWrite(pwm_1, Speed);
          digitalWrite(dir_1, HIGH);
          pub_flag.publish(&Flag); //Publish the Flag message
          delay(1000);
          if (Flag.data == 2){
               Flag.data = 0;// Reset the Flag
          }
    
}



// if the target in the middle stop **************
else{ 
     nh.loginfo("Stop");//print on terminal window
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

 Flag.data=0;

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

////Flag
//Flag.data=0;
pub_flag.publish(&Flag);

  
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
