// Setup to talk to the Serial Terminal when connected to the computer
#include <SoftwareSerial.h>
SoftwareSerial GPRS(7, 8);
unsigned char buffer[256];
int count = 0;

// Setting up inicial door state
int last_door_value = -1;
int current_door_value = -1;

int total_delay = 0;

// Iniciating communication with the Serial Terminal and the PINS that are going to send and receive the pulses to the remote
void setup()
{
  GPRS.begin(19200);
  Serial.begin(19200);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
}


void loop()
{
  current_door_value = digitalRead(5);
  
  if (last_door_value == -1) {
    // This will initialize our "What was the last thing we read?" variable
    last_door_value = current_door_value;
  }

  delay(1000);

  total_delay += 1000;

// This will send the SMS letting me know that a Door has been opened or closed
  if (current_door_value != last_door_value) {
    if (current_door_value == 0) {
      GPRS.println("AT+CMGF=1");
      delay(100);
      GPRS.println("AT+CMGS = \"+12674671270\"");
      delay(100);
      GPRS.println("Door Closed");
      delay(500);
      GPRS.println((char)26);
      delay(500);
      GPRS.println();
      delay(500);
      readPrint();
      clearBufferArray();
      delay(3500);
    } else {
      GPRS.println("AT+CMGF=1");
      delay(100);
      GPRS.println("AT+CMGS = \"+12674671270\"");
      delay(100);
      GPRS.println("Door Opened");
      delay(500);
      GPRS.println((char)26);
      delay(500);
      GPRS.println();
      delay(500);
      readPrint();
      clearBufferArray();
      delay(3500);
    }
    last_door_value = current_door_value;
  }

// Next several lines are for receiving the SMS and sending the signal to the Arduino to Start the car, Close or Open the door locks
  if (total_delay > 5000 && GPRS.available())
  {
    total_delay = 0;
    delay(5000);
    readPrint();
    clearBufferArray();

    delay(1000);
    GPRS.println("AT+CMGR=1");
    delay(2000);

    readPrint();

    String str1 = (const char*)buffer;
    int index = str1.lastIndexOf("\"");
    String text = str1.substring(index + 1, str1.length() - 1);
    String str2 = "Start";
    String str3 = "Open";
    String str4 = "Close";
    if (text.indexOf(str2) != -1)
    {
      //     Serial.write("\nStart\n");
      for (int x = 0; x < 2; x++)
      {
        digitalWrite(2, HIGH);
        delay(800);
        digitalWrite(2, LOW);
        delay(1500);
      }
      GPRS.println("AT+CMGF=1");
      delay(100);
      GPRS.println("AT+CMGS = \"+12674671270\"");
      delay(100);
      GPRS.println("Started");
      delay(500);
      GPRS.println((char)26);
      delay(500);
      GPRS.println();
      delay(500);
      readPrint();
      clearBufferArray();
      delay(3500);

    }
    else
    {
      if (text.indexOf(str3) != -1)
      {
        //     Serial.write("\nOpen\n");
        for (int x = 0; x < 2; x++)
        {
          digitalWrite(3, HIGH);
          delay(600);
          digitalWrite(3, LOW);
          delay(600);
        }
        GPRS.println("AT+CMGF=1");
        delay(100);
        GPRS.println("AT+CMGS = \"+12674671270\"");
        delay(100);
        GPRS.println("Unlock Succesful");
        delay(500);
        GPRS.println((char)26);
        delay(500);
        GPRS.println();
        delay(500);
        readPrint();
        clearBufferArray();
        delay(3500);
      }
      else
      {
        if (text.indexOf(str4) != -1)
        {
          //     Serial.write("\nClose\n");
          for (int x = 0; x < 2; x++)
          {
            digitalWrite(4, HIGH);
            delay(600);
            digitalWrite(4, LOW);
            delay(600);
          }
          GPRS.println("AT+CMGF=1");
          delay(100);
          GPRS.println("AT+CMGS = \"+12674671270\"");
          delay(100);
          GPRS.println("Lock Succesful");
          delay(500);
          GPRS.println((char)26);
          delay(500);
          GPRS.println();
          delay(500);
          readPrint();
          clearBufferArray();
          delay(3500);
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

void readPrint()  // This function reads and puts together everything on the SIM Card and prints it to the Serial Port
{
  while (GPRS.available())
  {
    buffer[count++] = GPRS.read();
    if (count == 256)break;
  }
  Serial.write(buffer, count);
}


void clearBufferArray()              // function to clear buffer array
{

  for (int i = 0; i < count; i++)
  {
    buffer[i] = (unsigned char)0;
  }
  count = 0;
}
