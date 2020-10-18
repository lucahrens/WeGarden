//This is the code for MKR!!!
#include "newconfig.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

byte h;
byte t;
int temps;
int humids;
int val = 0;
int soilPin = A0;
int soilPower = 7;
AdafruitIO_Feed *temp = io.feed("temp");
AdafruitIO_Feed *humid = io.feed("humid");
AdafruitIO_Feed *sunlightStrength = io.feed("sunlightStrength");
AdafruitIO_Feed *soilMoist = io.feed("soilMoist");
AdafruitIO_Feed *soilTemp = io.feed("soilTemp");

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(A0, INPUT);
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  // connect to io.adafruit.com
  io.connect();
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  Serial.println("Dallas Temperature IC Control Library Demo");

  sensors.begin();

  pinMode(soilPower, OUTPUT);
  digitalWrite(soilPower, LOW);
}
void loop() {
 io.run();
 int value = analogRead(A0);
 value = value/102;
  Serial.println("Analog value : ");
  Serial.println(value);
   //turns it into a scale from 1-10
  sunlightStrength->save(value);
  Serial1.readBytes(&t, 1);
  Serial1.readBytes(&h, 1); //Read the serial data and store in var
  if (t < h) {
    temps = t;
    humids = h;
  } else {
    temps = h;
    humids = t;
  }
  temps = temps * 1.8;
  temps = temps + 32;
  temp->save(temps);
  humid->save(humids);
  Serial.print ("H = "); 
  Serial.println(humids, DEC); //Print data on Serial Monitor
  Serial.print ("T = "); 
  Serial.println(temps, DEC); //Print data on Serial Monitor
  Serial.println("runing"); //Print data on Serial Monitor
  delay(10000);

//for soil temp
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature is: ");
  float temp = sensors.getTempCByIndex(0);
  temp = temp * 9/5 + 32;
  Serial.print(temp); // Why "byIndex"? 
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
    delay(1000);

//for soil moisture

  Serial.print("Soil Moisture = ");    
//get soil moisture value from the function below and print it
  Serial.println(readSoil());

//This 1 second timefrme is used so you can test the sensor and see it change in real-time.
//For in-plant applications, you will want to take readings much less frequently.
  delay(60000);// was 1000 //take a reading every second
}
//This is a function used to get the soil moisture content
  int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    val = val/90; // turns it into a 1-10 scale
    return val;//send current moisture value

    
}
