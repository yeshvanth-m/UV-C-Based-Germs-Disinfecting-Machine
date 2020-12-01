//  _    _         _               ___     ___ 
//  \\  //         ||       /\    ||  \\  //  \\
//   \\//    __    ||      /__\   ||__//  \\__  
//    //\\  //\\   ||     //  \\  ||  \\  _   \\
//   //  \\//  \\  ||||| //    \\ ||__//  \\__// 

// A YM Labs Production

//////////////////////////////////////////////
// For Stepper Motor -  Pins                //
   const int stepPin = 3;                   //
   const int dirPin = 4;                    //      
////////////////////////////////////////////// 
// For LCD                                  //
//  Libraries                               //  
    #include <Wire.h>                       //
    #include <LCD.h>                        //
    #include <LiquidCrystal_I2C.h>          //
//  I2C Object,                             //
LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7); //
//////////////////////////////////////////////
//  For Ultrasonic Sensor                   //
// Trigger pin and echo pin                 //
    const int pingPin = 7;                  //
    const int echoPin = 6;                  //
// Distance                                 //
    long duration, cm;                      //
//////////////////////////////////////////////
// For Limit switch                         //
// Setting pin and value                    //        
    const int b_ls = A3;                    //
    int bls_val = LOW;                      //   
//////////////////////////////////////////////
//  For IR Sensors                          //
//  Setting the pins and values             //
    const int ir1 = A4;                     //
    const int ir2 = A5;                     //
    int val1 = 0;                           //
    int val2 = 0;                           //
//////////////////////////////////////////////
//  For timers                              // 
    long int time1, time2;                  //
    int timer = 1000;                       //
    long int dis_time = 12500*60;           //
//////////////////////////////////////////////
//  For UV light                            //
    const int uv_pin = 1;                   //
//////////////////////////////////////////////
// For Servo                                //
   const int servopin = 9;                  //
//////////////////////////////////////////////

void setup() 
{

//////////////////////////////////////////////
//Setting up the Traverser                  //
//Setting Stepper Motor pins                //
  pinMode (stepPin,OUTPUT);                 //
  pinMode (dirPin,OUTPUT);                  //    
                                            //
//Enabling pulldown for Limit Switch        //
  pinMode (b_ls, INPUT_PULLDOWN);           //
                                            //
//Set direction to backward                 //
  digitalWrite (dirPin,LOW);                //
//------------------------------------------//
//Bring the traverser to the position       //
  while (bls_val == LOW)                    //
  {                                         //
    digitalWrite (stepPin,HIGH);            //
    delay (1);                              //
    digitalWrite (stepPin,LOW);             //
    delay (1);                              //
    //Continuously read switch value        //
    bls_val = digitalRead (b_ls);           //
  }                                         //
//////////////////////////////////////////////
// Set up the LCD                           //
  lcd.begin (16,2); // For 16*2 LCD         //
  lcd.clear ();                             //
  lcd.home ();                              //
  lcd.print ("UVC Based");                  //
  lcd.setCursor (0,1);                      //
  lcd.print ("Disinfector");                //
  delay(4000);                              //
//////////////////////////////////////////////
// Set up the Ultrasonic Sensor             //
  pinMode (pingPin, OUTPUT);                //
  pinMode (echoPin, INPUT);                 //    
//////////////////////////////////////////////
// Set up the IR sensors                    //
  pinMode (ir1, INPUT);                     //
  pinMode (ir2, INPUT);                     //
//////////////////////////////////////////////
// Set up UV light enable pin               //
  pinMode (uv_pin, OUTPUT);                 //
//////////////////////////////////////////////    
// Disable flip                             //
  pinMode( servopin, OUTPUT);               //
  digitalWrite(servopin, LOW);              //
//////////////////////////////////////////////
} 

void loop() 
{
  execute();
}



void execute()
{ 
  DisplayControl (1);      //Display open tray option
  delay(1000);
  sense_1();               //To sense sensor 1
    DisplayControl (2);    //Display status message
    TraverserControl (1);  //Open the tray
    DisplayControl (3);    //Display Close tray option
  sense_2();               //To sense sensor 2
    DisplayControl (9);    //Display status message
    TraverserControl (0);  //To close the tray
    DisplayControl (4);    //To display default time
    int val = sense_1_2_();//To sense both sensors for input
    if (val == 0)          //If user selects to 
    {  
      DisplayControl (5);  //Display disinfecting message
      disinfect();         //Start disinfection
    }
    else
    {
       changeTime();       //To change default timing
       DisplayControl (5); //Display disinfecting message 
       disinfect();        //Start disinfection
    }
  sense_1();
    DisplayControl (6);    //Display status message
    TraverserControl (1);  //To open the tray
  sense_1();
    DisplayControl (9);    //Display status message
    TraverserControl (0);  //To close the tray
    DisplayControl (7);    //Display thank you message
}

//To control the display
void DisplayControl(int a)
{
  lcd.clear ();
  lcd.home ();
  switch (a)
  {
    case 1:                              
      lcd.print ("Do not touch :)");                  
      lcd.setCursor (0,1);                      
      lcd.print ("1 to Open tray");
      break;
    case 2:
      lcd.print ("Opening tray");
      lcd.setCursor (0,1);                      
      lcd.print ("Please wait...");
      break;
    case 3:
      lcd.print ("Place the items");
      lcd.setCursor (0,1);
      lcd.print ("2 to close tray");
      break;
    case 4:
      lcd.print ("Default time:");
      lcd.setCursor (0,1);
      lcd.print ("12.5 Minutes");
      delay (4000);
      lcd.clear ();
      lcd.home();
      lcd.print ("1 to continue");
      lcd.setCursor (0,1);                      
      lcd.print ("2 to change time");
      break;
    case 5:
      lcd.print ("Disinfecting...");
      lcd.setCursor (0,1);                      
      lcd.print ("Emergency stop 1");
      break;
    case 6:
      lcd.print ("Opening tray...");
      lcd.setCursor (0,1);                      
      lcd.print ("1 to close tray");
      break;
    case 7:
      lcd.print ("Thank you for");
      lcd.setCursor (0,1);                      
      lcd.print ("using YM Labs DM");
      break; 
    case 8:
      lcd.print ("Invalid choice");
      lcd.setCursor (0,1);                      
      lcd.print ("Try again");
      break;  
    case 9:
      lcd.print ("Closing tray");
      lcd.setCursor (0,1);                      
      lcd.print ("Please wait...");
      break; 
      default:
      lcd.print ("Welcome to YM Labs!");       
  }
}


void disinfect()
{
  digitalWrite(uv_pin, HIGH);
  delay(dis_time/2);
  digitalWrite(servopin, HIGH);
  delay(1010);
  digitalWrite(servopin, LOW);
  delay(dis_time/2);
  digitalWrite(uv_pin, LOW);
}


// A simple recursive function to sense the right gesture of the hand on sensor 1
int sense_1()
{
  do 
  {
    val1 = digitalRead(ir1); 
  } while (val1 == 0);
    time1 = millis();
    while (val1 == 1) 
    {
      val1 = digitalRead(ir1);
      time2 = millis();
      if( (time2-time1) > timer) 
      { 
        return 0;
      }
    }
    sense_1();
}

// A simple recursive function to sense the right gesture of the hand on sensor 2
int sense_2()
{
  do 
  {
    val2 = digitalRead(ir2); 
  } while (val2 == 0);
    time1 = millis();
    while (val2 == 1) 
    {
      val2 = digitalRead(ir2);
      time2 = millis();
      if( (time2-time1) > timer) 
      { 
        return 0;
      }
    }
    sense_2();
}

// A simple recursive function to sense the right gesture of the hand on both sensors
int sense_1_2_()
{
  do
    {
      val1 = digitalRead(ir1); 
      val2 = digitalRead(ir2);
    } while (val1 == 0 && val2 == 0) ;
    if (val1 == 1 && val2 == 1)
    {
      DisplayControl (8);
      delay(2000);
      sense_1_2_();
    }
    time1 = millis();
    while (val1 == 1) 
    {
      val1 = digitalRead(ir1);
      time2 = millis();
      if( (time2-time1) > timer) 
      { 
        return 0;
      }
    }
    while (val2 == 1) 
    {
      val2 = digitalRead(ir2);
      time2 = millis();
      if ( (time2-time1) > timer) 
      {
        return 1;
      }
    }
    sense_1_2_();
}

// To control the stepper motor using the A4988 driver
void TraverserControl (int y)
{
  if (y == 0)
  {
    digitalWrite (dirPin, LOW);     // Set the directiom
    bls_val = digitalRead (b_ls);   // Read the limit switch
    while (bls_val == LOW)          // While traverser not at the end
    {
      digitalWrite (stepPin,HIGH); 
      delay (1); 
      digitalWrite (stepPin,LOW); 
      delay (1); 
      bls_val = digitalRead (b_ls); // Move the traverser and read the limit switch
    }
  }
  else
  {   
    digitalWrite (dirPin,HIGH);     // Set the directiom
    for (int x = 0; x < 6800; x++)  // Write the number of steps required to open the tray
    {
      digitalWrite (stepPin,HIGH); 
      delay (1); 
      digitalWrite (stepPin,LOW); 
      delay (1); 
    }
  }
}


//To change the time of disinfection
int changeTime()         
{
  while (val1 == 0) 
  {
    lcd.clear ();
    lcd.home ();
    lcd.print ("Timer: ");
    long dist = calcDistance();        // Calculate the distance 
// Set the timing in accordance with the distance between the hand and the sensor
    if (dist >= 1 && dist <= 5)        
    {
      lcd.print ("30 min.");
      dis_time = 1000*60*30;
    }
    else if (dist > 5 && dist <= 10)
    {
      lcd.print ("1 Hr.");
      dis_time = 1000*60*60;
    }
    else if (dist > 10 && dist <= 15)
    {
      lcd.print ("2 Hrs.");
      dis_time = 1000*60*60*2;
    }
    else if (dist > 15 && dist <= 20)
    {
      lcd.print ("4 Hrs.");
      dis_time = 1000*60*60*4;
    }
    else if (dist > 20 && dist <= 25)
    {
      lcd.print ("8 Hrs.");
      dis_time = 1000*60*60*8;
    }
    else if (dist > 25 && dist <= 30)
    {
      lcd.print ("12 Hrs.");
      dis_time = 1000*60*60*12;
    }
    else
    {
      lcd.print ("12.5 Min.");
      dis_time = 12500*60;
    }
    lcd.setCursor (0,1);                      
    lcd.print ("1 to set Timer"); 
    delay(500);
    val1 = digitalRead(ir1);
  }           // Repeat until the timer is set
  while (val1 == 1) 
  {
    lcd.clear ();
    lcd.home ();
    lcd.print("Timer Set");
    delay(2000);
    return 0;
  }
}

// To calculate the distance between the sensor and the hand
long calcDistance()       
{
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  return cm;
}

// To convert the microseconds to distance in cm
long microsecondsToCentimeters(long microseconds) 
{
   return microseconds / 29 / 2; 
}
