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
  "Aripaev         ",
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

// Pins for the bush buttons

const int button1Pin = 10;
const int button2Pin = 11;
const int button4Pin = 12;
const int button8Pin = 13;// the number of the pushbutton pin

int button1State = 0;
int button2State = 0;
int button4State = 0;
int button8State = 0;

int pinsState[4] = {0,0,0,0};

bool pinsPressed = false;

void vahetaJaama(int jaam){
  radio.setChannel(sagedused[jaam]);
  //Display station number and name
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Freq:"); display.println((String(sagedused[jaam]/10.0)+" "));
  display.setCursor(0,10);
  display.println(String(jaam+1)+". "+jaamanimed[jaam]);
  display.setCursor(0,30);
  display.print("Volume:"); display.println(volume);
  display.display();
}

void setup()
{
  Serial.begin(9600);
  
  radio.powerOn();
  radio.setVolume(volume);

  display.begin();
  display.setContrast(60);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(1);
  display.setTextColor(BLACK);

  vahetaJaama(4);

  pinMode(button1Pin, INPUT);
  digitalWrite(button1Pin, HIGH);
  pinMode(button2Pin, INPUT);
  digitalWrite(button2Pin, HIGH);
  pinMode(button4Pin, INPUT);
  digitalWrite(button4Pin, HIGH);
  pinMode(button8Pin, INPUT);
  digitalWrite(button8Pin, HIGH);
}

void loop()
{ 
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button4State = digitalRead(button4Pin);
  button8State = digitalRead(button8Pin);

  //Serial.println(button1State);
  
  if ((button1State==LOW) || (button2State==LOW) || (button4State==LOW) || (button8State==LOW)){
    if (button1State==LOW) {
      Serial.println("Vajutati nupp 1");
    }
    if (button2State==LOW) {
      Serial.println("Vajutati nupp 2");
    }
    if (button4State==LOW) {
      Serial.println("Vajutati nupp 4");
    }
    if (button8State==LOW) {
      Serial.println("Vajutati nupp 8");
    }
    
    pinsPressed = true;
    pinsState[0] = (pinsState[0] || (button1State==LOW));
    pinsState[1] = (pinsState[1] || (button2State==LOW));
    pinsState[2] = (pinsState[2] || (button4State==LOW));
    pinsState[3] = (pinsState[3] || (button8State==LOW));
  }
  else {
    if (pinsPressed){
      jaam = pinsState[0] + 2*pinsState[1] + 4*pinsState[2] + 8*pinsState[3]-1;
      Serial.println(jaam);
      //Change the station
      vahetaJaama(jaam);
      //Reset pins
      pinsState[0] = 0;
      pinsState[1] = 0;
      pinsState[2] = 0;
      pinsState[3] = 0;
      pinsPressed = false;
    }
  }
}

