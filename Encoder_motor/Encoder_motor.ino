#define encOutA 2 //CLK
#define encOutB 3 //DT

int counter=0,State,OldState;
String direction="";
float rpm;
unsigned long start=0;
float reduction_ratio;
float CPR=5/reduction_ratio; //Counts per rev(CPR)//number of poles for IG42 motor=5
float PPR=CPR/4;
const byte updateRate=2;
int UpdateTime_in_milli_sec=1000/updateRate;

void setup(){
  pinMode(encOutA,INPUT);
  pinMode(encOutB,INPUT);

  Serial.begin(9600);

  OldState=digitalRead(encOutA);

  // Call updateEncoder() when any high/low changed seen
  // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0,updateEncoder,CHANGE); 
  attachInterrupt(1,updateEncoder,CHANGE);
}

void loop(){
  if(millis()-start>UpdateTime_in_milli_sec){
    start=millis();
    rpm=counter*60*updateRate/(PPR*2);
    Serial.println(rpm);
    pulse=0;
  }
}

void updateEncoder(){ //Interrupt service routine
  state=digitalRead(encOutA);
  if(State!=OldState && State==1){
    if(digitalRead(encOutB)!=State){
      counter--;
      direction="Counter Clockwise";
    }
    else{
      counter++;
      direction="Clockwise";
    }
    Serial.print("Direction: ");
    Serial.print(direction);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }
  OldState=State;
}
