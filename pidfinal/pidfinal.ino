double Setpoint ; 
double Input; 
double Output ; //LED
//PID parameters
double Kp=0, Ki=10, Kd=0; //this is the tuning variables, we need to test the robot again and again to finely tune it.

/*You can define all the pins connectes to the sensors here
 * then just analogRead them in the setup
 * then we need the setpoint, it will be the value of the sensors just above the line that is just the middle value in the array of the sensor values
 * then we also need to calculate the average value of the sensor values
 * dividing the above two will give the position og the bot w.r.t the line
*/
long sensors_average=0;
int sensors_sum=0;
int Position;
int set_point, proportional,integral=0,derivative=0,last_proportional = 0,error_value;


void setup() {
  /*
   * here all the setup stuff can be written
   */
}

void loop() {
  /*
   *here the values of sensors_average and sensors_sum need to be calculated
   *but before the above point the sensors need to be read and the values need to be stored in an array of variables
   *then the functions below can be called for pid calculation
   */

   pid_calc();
   calc_turn();
   motor_drive(right_speed, left_speed);//by default the robot will move in max_speed, whenever there will be some disturbance, it will change its speed accordingly
}

void pid_calc() 
  {   Position = int(sensors_average / sensors_sum); 
      proportional = Position – set_point;      // Replace set_point by your set point 
      integral = integral + proportional; 
      derivative = proportional - last_proportional; 
      last_proportional = proportional;   
       
      error_value = int(proportional * Kp + integral * Ki + derivative * Kd); 
}

int max_speed,right_speed,left_speed;

void calc_turn() 
{  //Restricting the error value between +256. 
if (error_value< -256)     
  { 
error_value = -256; 
  } 
 
if (error_value> 256) 
  { 
error_value = 256; 
  } 
 
  // If error_value is less than zero calculate right turn speed values 
if (error_value< 0) 
  { 
right_speed = max_speed + error_value; 
left_speed = max_speed; 
  } 
  // If error_value is greater than zero calculate left turn values 
else 
  { 
right_speed = max_speed; 
left_speed = max_speed - error_value; 
  } 
} 

void motor_drive(int Right_speed, int Left_speed) 
{    // Drive motors according to the calculated values for a turn 
analogWrite(motor_right, right_speed); 
analogWrite(motor_left, left_speed); 
delay(50);           // Optional 
}
