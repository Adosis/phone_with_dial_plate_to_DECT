
int pinMatrix[]={23,22,32,33,25,26,27,14,12,13};
int pinDialProcess = 5;             // input for detectint dialling process
int pinCall = 19;
int pinDialPulse = 18;              // input for dialling pulses
int pinFork=17;                     // input for switch of phone fork 
int pinRing=21;                     // output for the bell
bool dialProcess=false;             // indicates a dialling process
bool impuls;                        // impuls of the dialling plate
bool impulssave=false;              // help variable to detect changes of the dialling impulse (Chang from HIGH to LOW and from LOW th HIGH)
bool pickedUp=false;                // help variable to detect the process of picking up and hanging up;  
bool incomingCall=false;            // High when incoming call was detected
bool stillCall=false;               // LED for incoming call is blinking; this variable indicates if the LED is still blinking or if the LED is out for a longer time
int number;                         // variable for counting the pulses while dialling; after dialling process is this the dialled number
char cmd;                           // char when using the serial prot for input
char key;                           // dialled number as char to transfer to calcMatrix()



void setup() 
{
  Serial.begin(9600);
  // all pins for the button matrix as OUTPUT
  for (int i=0;i<=9;i++)
  {
    pinMode (pinMatrix[i],OUTPUT);
  }  
  pinMode(pinRing,OUTPUT);                //pin for the bell as OUTPUT
  pinMode(pinDialPulse, INPUT_PULLUP);    //pin for dialling impulse as INPUT with pullup resistant
  pinMode(pinDialProcess, INPUT_PULLUP);  //pin for dialling process as INPUT with pullup resistant
  pinMode(pinFork, INPUT_PULLUP);         //pin for button of phone fork as INPUT with pullup resistant
  pinMode(pinCall, INPUT_PULLUP);         //pin for incoming call as INPUT with pullup resistant  
  number=0;   //reset the variable for counting the dialling pulses
}



void loop() 
{  
  if(Serial.available())      // check, if there was an input by serial port
  {
    cmd = Serial.read();      // transfer the character to the variable 'cmd'
    matrixCalc(String(cmd));  // call the function 'matrixCalc()' to send the character to the DECT
  }
  if (digitalRead(pinCall)==LOW)    // detecting an incoming call
  {
    Serial.println("ringing...");
    incomingCall=true;
    ring();                         // call the function 'ring()'
  }
  // no incoming call; to check if there is really no incoming signal or if the blinking LED is just off input will be checked in a loop
  if ((digitalRead(pinCall)==HIGH) && (incomingCall==true))   
  {
    stillCall=false;
    for (int c=0;c<=100;c++)
    {
      if (digitalRead(pinCall)==LOW)   // still detect incoming call, 
      {
        stillCall=true;
      } 
      delay(10); 
    }
    if (stillCall==false)               
    {
      Serial.println("ringing off");        
      incomingCall=false;
    }
  }
  if (digitalRead(pinFork)==LOW and pickedUp==false)   // is there a process of picking up the phone?
  {
    pickedUp=true;
    Serial.println("picked up");    
    matrixCalc("h");    // Abheben
  }
  if (digitalRead(pinFork)==HIGH and pickedUp==true)  // is there a process of hanging up the phone?
  {
    pickedUp=false;
    Serial.println("hanged up");    
    matrixCalc("l");      // send signal for hanging up
  }  
  if (pickedUp == true)   // when picked up wait for dialling process
  {
    if (digitalRead(pinDialProcess)==LOW)    // detect a dialling process
    {
      dialProcess=true;      
      delay (25);
      impuls = digitalRead(pinDialPulse); 
      if (impuls != impulssave)             // every change of the dialling pulse will be counted
      {
        impulssave = impuls;
        if (impuls == 0){number=number+1;}
        Serial.print (impuls);   
        Serial.print ("_");
        delay(10);
      }
    }
    if (digitalRead(pinDialProcess)==HIGH && dialProcess==true)   // finish of dialling process of the current number 
    {
      dialProcess=false;
      Serial.print("    number: ");
      Serial.println(number);
      delay (15);
      matrixCalc(String(number));
      number=0;
    }
  }  
  if (pickedUp == false)   // when hanged up wait for dialling process; this means that steps will follow connecting the DECT phone to a station
  {
    if (digitalRead(pinDialProcess)==LOW)    // detect a dialling process
    {
      dialProcess=true;      
      delay (25);
      impuls = digitalRead(pinDialPulse); 
      if (impuls != impulssave)             // every change of the dialling pulse will be counted
      {
        impulssave = impuls;
        if (impuls == 0){number=number+1;}
        Serial.print (impuls);   
        Serial.print ("_");
        delay(10);
      }
    }
    if (digitalRead(pinDialProcess)==HIGH && dialProcess==true)   // finish of dialling process of the current number 
    {
      dialProcess=false;
      Serial.print("    number: ");
      Serial.println(number);
      delay (15);
      // the menue of the DECT phone changed, when connected to the basis or to a fritz box
      // so, there are two routines to connect the phone
      // in case of the number which was dialled, the routine will be started
      // 9 for connecting to a basis
      // 0 for connecting to a fritzBox
      if (number==9)
      {
        Serial.println("Connecting phone to basis...");
        linkBasis();
      }
      if (number==10)
      {
        Serial.println("Connecting phone to fritzBox...");
        linkFritz();
      }
      number=0;      
    }
  }      
}

// routine to connect the DECT phone to the basis
void linkBasis ()
{
  Serial.println("phone will be connected to basis!");
  matrixCalc("m");
  matrixCalc("d");
  matrixCalc("d");
  matrixCalc("m");
  matrixCalc("m");
  matrixCalc("m");
  matrixCalc("0");
  matrixCalc("0");
  matrixCalc("0");
  matrixCalc("0");
  matrixCalc("m");  
}



void linkFritz ()
// routine to connect the DECT phone to the fritzBox
{
  Serial.println("phone will be connected to fritzBox!");
  matrixCalc("m");
  matrixCalc("d");
  matrixCalc("d");   
  matrixCalc("d");
  matrixCalc("m");
  matrixCalc("d");
  matrixCalc("d");   
  matrixCalc("d");
  matrixCalc("d");
  matrixCalc("d");   
  matrixCalc("d");
  matrixCalc("d");
  matrixCalc("d");      
  matrixCalc("m");
  matrixCalc("m");
  matrixCalc("0");
  matrixCalc("0");
  matrixCalc("0");
  matrixCalc("0");
  matrixCalc("m");  
}


// function to calculate the button matrix
void matrixCalc(String key)
{
  Serial.print ("character: "); 
  Serial.println (key);  
  if ((key=="10") || (key=="0")) {matrixWrite (0,6);}  
  if (key=="1") {matrixWrite (1,6);}
  if (key=="2") {matrixWrite (3,6);}    
  if (key=="3") {matrixWrite (4,8);}
  if (key=="4") {matrixWrite (1,7);}
  if (key=="5") {matrixWrite (4,9);}
  if (key=="6") {matrixWrite (0,9);}
  if (key=="7") {matrixWrite (4,7);}
  if (key=="8") {matrixWrite (2,7);}
  if (key=="9") {matrixWrite (3,9);}
  if (key=="h") {matrixWrite (2,8);}  // pick up
  if (key=="l") {matrixWrite (3,8);}  // hang up
  if (key=="m") {matrixWrite (0,8);}  // menu
  if (key=="s") {matrixWrite (0,7);}  // mute  
  if (key=="u") {matrixWrite (1,9);}  // up
  if (key=="d") {matrixWrite (2,9);}  // down  
 }



void matrixWrite (int a,int b)
// funtion to send an signal (pulse) to the DECT button matrix
{ 
  digitalWrite (pinMatrix[a], HIGH);  
  digitalWrite (pinMatrix[b],HIGH);
  delay (125);
  digitalWrite (pinMatrix[a], LOW);  
  digitalWrite (pinMatrix[b],LOW);
  delay (125);
}  


void ring()
// function to let the bell ring
{  
  for (int i=0;i<=125;i++)
  {
    digitalWrite (pinRing,!digitalRead(pinRing));
    delay(15);   
  }
  digitalWrite (pinRing,LOW);   
  delay(1500);
}
