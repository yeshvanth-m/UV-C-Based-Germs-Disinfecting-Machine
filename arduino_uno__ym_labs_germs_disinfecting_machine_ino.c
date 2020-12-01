//  _    _         _               ___     ___ 
//  \\  //         ||       /\    ||  \\  //  \\
//   \\//    __    ||      /__\   ||__//  \\__  
//    //\\  //\\   ||     //  \\  ||  \\  _   \\
//   //  \\//  \\  ||||| //    \\ ||__//  \\__// 

// A YM Labs Production

//////////////////////////////////////////////
// For Servo Motor                          //  
//   Library                                //
     #include <Servo.h>                     //
//   Objects                                //
     Servo servo1;                          //
     Servo servo2;                          //      
//   Servo Positions                        //
     int pos1 = 0;                          //                   
     int pos2 = 0;                          //
//////////////////////////////////////////////
// For input from MKR WiFi 1010             //
     const int input = 12;                  // 
     int value = 0;                         //
//////////////////////////////////////////////
//  For timers                              // 
    long int time1, time2;                  //
    int timer = 1000;                       //
//////////////////////////////////////////////
void setup()
{
//////////////////////////////////////////////
// Setting up servo connection              //
   servo1.attach (3);                       //
   ServoControl (0);                        //
//////////////////////////////////////////////  
}


void loop()
{
  // Continuously check for input from arduino
  do 
  {
    value = digitalRead(input); 
  } while (value == 0);
  time1 = millis();
  while (value == 1) 
  {
    value = digitalRead(input);
    time2 = millis();
    if( (time2-time1) > timer) 
    { 
      flip();
      break;
    }
  }
}

// To flip the items
void flip()
{
  ServoControl(1);
  delay(1000);
  ServoControl(2);
  delay(1000);
}

// To make the servo to go from one angle to another
void ServoControl (int x)                   
{                                                                                      
  if (x == 1)                           
  { 
    pos2 = 166;                                       
    for (pos1 = 178; pos1 >= 30; pos1 -= 1)  
    {                                        
      servo1.write (pos1);
      servo2.write (pos2);                  
      delay (15);                            
    }    
    delay(250);
    for (pos1 = 30; pos1 <= 178; pos1 += 1)  
    {                                        
      servo1.write (pos1);      
      servo2.write (pos2);             
      delay (15);                           
    } 
    delay(250);
  }                                      
  else if (x == 2)                               
  {  
    servo2.attach(6);
    pos1 = 178;                                        
    for (pos2 = 166; pos2 >= 30; pos2 -= 1)
    {     
      servo2.write (pos2);
      servo1.write (pos1);
      delay (15);  
    }
    delay(250);                                      
    for(pos2 = 30; pos2 <= 166; pos2 += 1)   
    {                                        
      servo2.write (pos2); 
      servo1.write (pos1);                  
      delay (15);                            
    }    
    delay(250);    
    servo2.detach();                                
  }
  else
  {
    servo1.write (178);
    delay (1000);
    servo2.attach(6);
    servo2.write (166);
    delay (1000);
    servo2.detach();
  }
}
