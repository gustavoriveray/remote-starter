// Setup to talk to the Serial Terminal when connected to the computer
#include <SoftwareSerial.h>
SoftwareSerial GPRS(7, 8);
unsigned char buffer[256]; 
int count=0;      


// Iniciating communication with the Serial Terminal and the PINS that are going to send the pulses to the remote
void setup()
{
 GPRS.begin(19200);                  
 Serial.begin(19200);            
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
}


void loop()
{
 delay(10000);
 if (GPRS.available())  // If there's data in the SIM Card
 {
  delay(5000);
  readPrint();
  clearBufferArray();
  
  delay(1000);
  GPRS.println("AT+CMGR=1");  //Get the message received
  delay(2000);
   
  readPrint();
  
// After getting the content of the message we look for the text
  String str1 = (const char*)buffer;
  int index = str1.lastIndexOf("\"");
  String text = str1.substring(index+1, str1.length()-1);
  String str2 = "Turn On";
  String str3 = "Open";
  String str4 = "Close";
  if (text.indexOf(str2) != -1)  //If text was Turn On
  {
     Serial.write("\nStart\n");  //Send text "Start" to the Serial Terminal
//Then do the double pulse to the PIN 2
     for (int x=0; x<2;x++)
     {
     digitalWrite(2, HIGH);
     delay(800);
     digitalWrite(2, LOW);    
     delay(1500);
     } 
  }
  else
  {
    if (text.indexOf(str3) != -1)  //If text was Open
  {
     Serial.write("\nOpen\n");   //Send text "Open" to the Serial Terminal
//Then do the double pulse to the PIN 3
     for (int x=0; x<2;x++)
     {
     digitalWrite(3, HIGH);
     delay(600);
     digitalWrite(3, LOW);    
     delay(600);
     } 
  }
  else
  {
  if (text.indexOf(str4) != -1)   //If text was Open
  {
     Serial.write("\nClose\n");   //Send text "Close" to the Serial Terminal
//Then do the double pulse to the PIN 4
     for (int x=0; x<2;x++)
     {
     digitalWrite(4, HIGH);
     delay(600);
     digitalWrite(4, LOW);    
     delay(600);
     } 
  }
  else
  Serial.write("\nnot found\n");  // If none was found print "not found" to the Serial Terminal
 }
}
    
  clearBufferArray();
  
  delay(1000);
  GPRS.println("AT+CMGD=1,4");  //Delete all messages in the SIM Card
  delay(1000);
  
    readPrint();
    clearBufferArray();
 }
}

//This function reads and puts together everything on the SIM Card and prints it to the Serial Port
void readPrint()
{
     while(GPRS.available())           
   {
     buffer[count++]=GPRS.read();    
     if(count == 256)break;
    }
   Serial.write(buffer,count);           
}


void clearBufferArray()              // function to clear buffer array
{
 
 for (int i=0; i<count;i++)
   { buffer[i]=NULL;}                  
  count = 0; 
}