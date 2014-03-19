#include "Queue.h"
#include <SPI.h>
#include <WiFi.h>
#include <SoftwareSerial.h>
#include "speedmeter.h"
#include "sevensegment_display.h"
#include "speaker.h"
#include "jingles.h"
#include "WiflyShield.h"

#define screenDataPin 11
#define screenClockPin 13
#define screenLatchPin 12
#define screenSize 5
#define speakerPin 9
#define blueStartPin 0
#define blueStopPin 1
#define redStartPin 2
#define redStopPin 3
#define blueIndicatorPin 4
#define redIndicatorPin 9
#define lineDistance 16.8
#define ssid "SchnapsWG"
#define password "Bieristetwasgutes8048"
#define wlanRxPin 8
#define wlanTxPin 9
#define scoreHost "localhost"
#define scoreUrl "/score/index.php"

Display *screen;
Speaker *speaker;
SpeedMeter *blue;
SpeedMeter *red;
WiflyShield *wlan;

Queue *runQueue;
int irStatusRedStart[6], irStatusRedStop[6], irStatusBlueStart[6], irStatusBlueStop[6];
boolean irShowStatus;

// ISR callbacks: callback to class method does not work
void redStartISR(){ red->startBarrierCrossedCallback(); }
void redStopISR(){ red->stopBarrierCrossedCallback(); }
void blueStartISR(){ blue->startBarrierCrossedCallback(); }
void blueStopISR(){ blue->stopBarrierCrossedCallback(); }
//attach the interrupts
void prepareRedISR(){
  attachInterrupt(red->pinToInterrupt(redStartPin), redStartISR, FALLING);
  attachInterrupt(red->pinToInterrupt(redStopPin), redStopISR, FALLING);
}
void prepareBlueISR(){
  attachInterrupt(blue->pinToInterrupt(blueStartPin), blueStartISR, FALLING);
  attachInterrupt(blue->pinToInterrupt(blueStopPin), blueStopISR, FALLING);
}
int irWatchdog(unsigned long now) 
{
  int redStart,redStop,blueStart,blueStop;
  
  redStart=redStop=blueStart=blueStop=0;
  for(int index = 6-1; index > 0; index--){
    irStatusRedStart[index] = irStatusRedStart[index-1];
    irStatusRedStop[index] = irStatusRedStop[index-1];
    //irStatusBlueStart[index] = irStatusBlueStart[index-1];
    //irStatusBlueStop[index] = irStatusBlueStop[index-1];
    redStart += irStatusRedStart[index];
    redStop += irStatusRedStop[index];
    //blueStart += irStatusBlueStart[index];
    //blueStop += irStatusBlueStop[index];
  }
  
  irStatusRedStart[0]=digitalRead(red->startPin);
  irStatusRedStop[0]=digitalRead(red->stopPin);
  //irStatusBlueStart[0]=digitalRead(blue->startPin);
  //irStatusBlueStop[0]=digitalRead(blue->stopPin);

  redStart += irStatusRedStart[0];
  redStop += irStatusRedStop[0];
  //blueStart += irStatusBlueStart[0];
  //blueStop += irStatusBlueStop[0];

  if(redStart==0 || redStop==0 ){//|| blueStart==0 || blueStop==0){
    irShowStatus = true;
  }
  if(redStart==6 && redStop==6){// && blueStart==6 && blueStop==6){
    if(irShowStatus==true){
      screen->clear();
      screen->writeString("     ");
    }
    
    irShowStatus = false;
  }
  if(irShowStatus){

    screen->writeCharacter('I', 0);
    screen->writeCharacter('R', 1);
    screen->writeCharacter(' ', 2);
    byte b1 = B01000100;
    byte b2 = B01000100;

    if(irStatusRedStart[0]==0){
      b1 = b1 & (~(1<<6));
    }
    if(irStatusRedStop[0]==0){
      b1 = b1 & (~(1<<2));
    }
    screen->writeByte(b1, 3);
    /*
    if(irStatusBlueStart[0]==0){
      b2 = b2 & (~(1<<6));
    }
    if(irStatusBlueStop[0]==0){
      b2 = b2 &  (~(1<<2));
    }
    screen->writeByte(b2, 4);*/
  }
}
int screenHandler(unsigned long now) 
{
  if(!irShowStatus){
  screen->update(now);
  }
  
}
int barrierHandler(unsigned long now)
{
  if(!irShowStatus){
  
  //wlan->readChar();
  
  boolean redBarrierCrossed = red->barrierCrossed();
  boolean blueBarrierCrossed = false;//blue->barrierCrossed();
  
  if(redBarrierCrossed || blueBarrierCrossed){
    //disable ISR
    //red->terminate();
    //blue->terminate();
    
    handleBarrierCrossed(redBarrierCrossed, blueBarrierCrossed);
    //enable ISR
    //prepareRedISR();
    //prepareBlueISR();
  }
  }

}

void setup()
{
 screen = new SevenSegmentDisplay(screenDataPin, screenClockPin, screenLatchPin, screenSize);
  screen->clear();
  screen->writeString("HOI");
  delay(1000);
  screen->writeString("    ");
  /*
  
  
  speaker = new Speaker(speakerPin);
  speaker->play(coin, sizeof(coin));
   */
  
  red = new SpeedMeter(redStartPin, redStopPin, redIndicatorPin, lineDistance);
  //blue = new SpeedMeter(blueStartPin, blueStopPin, blueIndicatorPin, lineDistance);
  prepareRedISR();
  //prepareBlueISR();
  
  //wlan = new WiflyShield(wlanRxPin, wlanTxPin, ssid, password);
//Serial.begin(9600);
runQueue = new Queue();
runQueue->scheduleFunction(screenHandler, "screen", 500, 100);
runQueue->scheduleFunction(barrierHandler, "barrier", 1000, 25);
runQueue->scheduleFunction(irWatchdog, "irwatch", 1500, 333);
while(1) {
  
  runQueue->Run(millis());

  delay(10);
}
  
}


void handleBarrierCrossed(boolean redBarrierCrossed, boolean blueBarrierCrossed){
  //char kmh[5];
  
  screen->writeString("GOAL!");
  //speaker->play(coin, sizeof(coin));
  delay(1000);
  if(redBarrierCrossed){
    //Serial.println(red->speed);
    screen->writeString(" RED ");
    delay(1000);
    screen->writeDouble(red->speed);
    //dtostrf(red->speed, 2, 2, kmh);
  }
  if(blueBarrierCrossed){
    screen->writeString("BLUE ");
    delay(1000);
    screen->writeDouble(blue->speed);
    //dtostrf(blue->speed, 2, 2, kmh);
    
  }
  
  //kmh[4]= '\0';
  ///screen->writeString(kmh);
}


void loop() {}


