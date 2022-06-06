//Line follower code for Roboassist line following bot 
//Sensor used for coding is  Advanced Auto-Calibrating Line Sensor (LSA08)
//More info about connections at:https://tutorial.cytron.io/2015/07/31/line-following-robot-using-lsa08-serial-mode-digital-output/
//Upload the sketch commandTest.ino into your Arduino board, 
//and it will help you to configure the LSA08 into a setting that is suitable for our line following task to work.


char address = 0x01;    // UART address as 0x01

void setup() {
  Serial.begin(9600);   // Start serial communication
  Serial.flush();   // Clear serial buffer

}

void loop() {

  char command, data;

  // Clear internal junction count of LSA08
  command = 'X';
  data = 0x00;
  sendCommand(command,data);

  // Setting LCD contrast to 90
  command = 'S';
  data = 90;
  sendCommand(command,data);

  // Setting LCD backlight to level 5
  command = 'B';
  data = 0x05;
  sendCommand(command,data);

  // Setting junction width to 6
  command = 'J';
  data = 0x06;
  sendCommand(command,data);

  // Setting threshold value to 5
  command = 'T';
  data = 0x05;
  sendCommand(command,data);

  // Setting line mode to Dark-On
  command = 'L';
  data = 0x01;
  sendCommand(command,data);

  // Setting UART ouput to mode 1
  command = 'D';
  data = 0x01;
  sendCommand(command,data);

  // Start calibration
  command = 'C';
  data = 0x00;
  sendCommand(command,data);

  while(1);   // Stay here to prevent infinite loop

}

/*
 * Function to send command to LSA08 in 4 continuous bytes.
 * LSA08 will reply with "OK" for every successful command sent.
 * However, reading the reply is optional, and thus not showing here.
 */
void sendCommand(char command, char data) {
  
  char checksum = address + command + data;
  
  Serial.write(address);
  Serial.write(command);
  Serial.write(data);
  Serial.write(checksum);

}
