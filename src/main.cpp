#include <Arduino.h>

#include <SparkFunSi4703.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;
int STC = 3;


Si4703_Breakout radio(resetPin, SDIO, SCLK, STC);
int channel;
int volume = 1;
char rdsBuffer[10];

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 8, 7, 6, 5);

const int jaamasid=15;

const int sagedused[jaamasid] = { 
  890,
  897,
  944,
  952,
  972,
  977,
  986,
  994,
  1002,
  1030,
  1036,
  1042,
  1047,
  1057,
  1061};
                     
const char* jaamanimed[jaamasid]={
  "Tartu Pereraadio",
  "Power Hit Radio ",
  "Raadio 4        ",
  "Sky Plus        ",
  "Raadio Uuno     ",
  "Äripäev         ",
  "Retro FM        ",
  "Star FM         ",
  "Raadio Kuku     ",
  "Hit FM          ",
  "Raadio 2        ",
  "Rock FM         ",
  "Ring FM         ",
  "Klassikaraadio  ",
  "Vikerraadio     "};

int jaam = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nSi4703_Breakout Test Sketch");
  Serial.println("===========================");  
  Serial.println("a b     Favourite stations");
  Serial.println("+ -     Volume (max 15)");
  Serial.println("u d     Seek up / down");
  Serial.println("r       Listen for RDS Data (15 sec timeout)");
  Serial.println("Send me a command letter.");
  
  radio.powerOn();
  radio.setVolume(volume);

  display.begin();
  display.setContrast(60);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(1);
  display.setTextColor(BLACK);
}

void displayInfo()
{
  Serial.print("Channel:"); Serial.print(channel); 
  Serial.print(" Volume:"); Serial.println(volume); 

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Channel:"); display.println(channel);
  display.setCursor(0,10);
  display.print("Volume:"); display.println(volume);
  display.display();
}

void loop()
{ 
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'u') 
    {
      channel = radio.seekUp();
      displayInfo();
    } 
    else if (ch == 'd') 
    {
      channel = radio.seekDown();
      displayInfo();
    } 
    else if (ch == '+') 
    {
      volume ++;
      if (volume == 16) volume = 15;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == '-') 
    {
      volume --;
      if (volume < 0) volume = 0;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == 'a')
    {
      channel = 930; // Rock FM
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'b')
    {
      channel = 974; // BBC R4
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'r')
    {
      Serial.println("RDS listening");
      radio.readRDS(rdsBuffer, 2000);
      Serial.print("RDS heard:");
      Serial.println(rdsBuffer);      
    }
  }
}

