// fuer temp und rh
#include "DHT.h"
#define DHTPIN A15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//fuer moisture
int sensorPin = A14;
int sensorValue = 0;

// fuer tft screen
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#define cs   10
#define dc   9
#define rst  8
TFT TFTscreen = TFT(cs, dc, rst);     // create an instance of the library
char sensorPrintouth[10];               // char array to print humidity
char sensorPrintoutm[4];               // char array to print moisture 
char sensorPrintoutt[10];               // char array to print temperature

//button
int buttonWater = 22;

void setup() {
    Serial.begin(9600); 
    Serial.println("Raumluftfeuchte, Temperatur und Feuchtigkeit der Erde");

    pinMode(buttonWater, INPUT);

    dht.begin();

    // tft screen
    // Put this line at the beginning of every sketch that uses the GLCD:
    TFTscreen.begin();

    // clear the screen with a black background
    TFTscreen.background(0, 0, 0);

    // write the static text to the screen
    // set the font color to white
    TFTscreen.stroke(255, 255, 255);
    // set the font size
    TFTscreen.setTextSize(2);
    // write the text to the top left corner of the screen
    TFTscreen.text("Humidity :\n ", 0, 0);
    TFTscreen.text("Temperature :\n", 0, 45);
    TFTscreen.text("Moisture :\n", 0, 90);
    // set the font size very large for the loop
    TFTscreen.setTextSize(2);


    //Setup Channel A, MotorShield
    pinMode(12, OUTPUT); //Initiates Motor Channel A pin
    pinMode(9, OUTPUT); //Initiates Brake Channel A pin
}

void loop() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    // sensorValue = analogRead(sensorPin); 
    delay(1000);

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
        Serial.print(" \t");
        Serial.print("Moisture: "); 
        Serial.print(sensorPrintoutm);

        //anzeige am tft screen                             110=moisture, 20=humi, 65=temp
        String sensorValm = String(analogRead(sensorPin));    // Read the value of the sensor on A0
        sensorValm.toCharArray(sensorPrintoutm, 4);           // convert the reading to a char array  moisture
        dtostrf(h, 7, 2, sensorPrintouth);
        dtostrf(t, 7, 2, sensorPrintoutt);
        TFTscreen.stroke(255, 255, 255);                      // set the font color
        TFTscreen.text(sensorPrintoutm, 50, 110);
        TFTscreen.text(sensorPrintouth, 20, 20);
        TFTscreen.text(sensorPrintoutt, 20, 65);
        delay(2500);
        TFTscreen.stroke(0, 0, 0);                            // erase the text you just wrote
        TFTscreen.text(sensorPrintoutm, 50, 110);
        TFTscreen.text(sensorPrintouth, 20, 20);
        TFTscreen.text(sensorPrintoutt, 20, 65);
    }




    //wasserpumpe fuer 5s einschalten wenn moisture unter 200 (sensorPrintoutm)
    if (analogRead(sensorPin) <= 200)
      {
        digitalWrite(12, HIGH);
        digitalWrite(9, LOW);
        analogWrite(3, 255);
        delay(5000);
        analogWrite(3, 0);
        digitalWrite(9, HIGH);
      }
}
