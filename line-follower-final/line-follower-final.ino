//  https://tutorial.cytron.io/2015/07/31/line-following-robot-using-lsa08-serial-mode-digital-output/ link for reference
//  some of the given values and variables are dependent on the hardware to be used and must be tested properly

const byte rx = 0;    // Defining pin 0 as Rx
const byte tx = 1;    // Defining pin 1 as Tx
const byte serialEn = 2;    // Connect UART output enable of LSA08 to pin 2
const byte junctionPulse = 4;   // Connect JPULSE of LSA08 to pin 4
const byte dir1 = 13;   // Connect DIR1 of motor driver to pin 13
const byte dir2 = 12;   // Connect DIR2 of motor driver to pin 12
const byte pwm1 = 11;   // Connect PWM1 of motor driver to pin 11 //left motor
const byte pwm2 = 10;   // Connect PWM2 of motor driver to pin 10 //right motor

double Kp=0, Ki=10, Kd=0;//this is the tuning variables, we need to test the robot again and again to finely tune it.
int set_point=35, proportional=0,integral=0,derivative=0,last_proportional = 0,error_value;
int base_speed=150, left_speed, right_speed, max_speed=220; //  base_speed and max_speed can be adjusted based on our requirements

void setup() {
  pinMode(serialEn,OUTPUT);   // Setting serialEn as digital output pin
  pinMode(junctionPulse,INPUT);   // Setting junctionPulse as digital input pin
  
  // Setting pin 0 - 7 as digital input pin
  for(byte i=0;i<=7;i++) {
    pinMode(i,INPUT);
  }
  
  // Setting pin 10 - 13 as digital output pin
  for(byte i=10;i<=13;i++) {
    pinMode(i,OUTPUT);
  }

  digitalWrite(serialEn,HIGH);
  
  // Setting the initial condition of motors
  // make sure both PWM pins are LOW
  digitalWrite(pwm1,LOW);
  digitalWrite(pwm2,LOW);
  
  // State of DIR pins are depending on your physical connection
  // if your robot behaves strangely, try changing thses two values
  digitalWrite(dir1,LOW);
  digitalWrite(dir2,LOW);

}

void loop() {
  digitalWrite(serialEn,LOW);
  int Position=Serial.read(); //  returns a value between 0 and 70 that indicates position of bot wrt line
  digitalWrite(serialEn,HIGH);
  pid(Position);  //  pid function
  
  // Checking for junction crossing, if juction detected, 
  // keep moving forward
  if(digitalRead(2) && digitalRead(5))
  moveForward();
  
  // Checking for sensor number 1 and 2, if line detected, move left
  else if(digitalRead(1) || digitalRead(2))
  moveLeft();

  // Checking for sensor number 5 and 6, if line detected, move right
  else if(digitalRead(5) || digitalRead(6))
  moveRight();

  // Checking for sensors number 3 and 4, 
  // if line is detected by either of these sensor, move forward
  else if(digitalRead(3) || digitalRead(4))
  moveForward();

  // If no line is detected, stay at the position
  else
  wait();

  // Put some delay to avoid the robot jig while making a turn
  delay(100);

}


// The values work good in my case, you could use other values set
// to archieve a performance that satisfy you
void moveLeft() {
  // For robot to move left, right motor has to be faster than left motor
  analogWrite(pwm1,10);
  analogWrite(pwm2,90);
}

void moveRight() {
  // For robot to move right, left motor has to be faster than right motor
  analogWrite(pwm1,90);
  analogWrite(pwm2,10);
}

void moveForward() {
  // For robot to move forward, both motors have to be same speed
  analogWrite(pwm1,base_speed);
  analogWrite(pwm2,base_speed);
}

void wait() {
  // Function to makes the robot stay
  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
}

void pid(int Position)
{
  proportional = Position - set_point;  // set_point is set as 35 i.e centre of the bot
  integral = integral + proportional; 
  derivative = proportional - last_proportional; 
  last_proportional = proportional;   
  error_value = int(proportional * Kp + integral * Ki + derivative * Kd);  

  if(error_value>0) // adding error_value with right or left motor should be tested 
  {
    right_speed = base_speed + error_value;
    left_speed = base_speed;
    if(right_speed > max_speed)
    {
      right_speed = max_speed;
    }
  }
  else // subtracting error_value from right or left motor should be tested  
  {
    right_speed = base_speed;
    left_speed = base_speed - error_value;
    if(left_speed>max_speed)
    {
      left_speed = max_speed;
    }
  }
  analogWrite(pwm1,left_speed);
  analogWrite(pwm2,right_speed);
}
