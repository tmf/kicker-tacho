#include "Arduino.h"

class SpeedMeter 
{
  static SpeedMeter *instance;
  static void startISR();
  static void stopISR();
  private:
    
    int indicatorPin;
    volatile double distance;
    volatile unsigned long startTime;
    volatile unsigned long stopTime;
    volatile unsigned long deltaTime;
    volatile boolean barrierFlag;
    
  public:
    int startPin;
    int stopPin;
    SpeedMeter(int start_p, int stop_p, int indicator_p, double d);
    boolean barrierCrossed();
    boolean checkBarrier();
    void startBarrierCrossedCallback();
    void stopBarrierCrossedCallback();
    void prepare();
    void terminate();
    int pinToInterrupt(int p);
    volatile double speed;
    volatile int timesCrossedStartBarrier;
    volatile int timesCrossedStopBarrier;
};

// for use by ISR glue routines
SpeedMeter * SpeedMeter::instance;


SpeedMeter::SpeedMeter(int start_p, int stop_p, int indicator_p, double d) : startPin(start_p), 
                                                                             stopPin(stop_p), 
                                                                             indicatorPin(indicator_p), 
                                                                             distance(d), 
                                                                             barrierFlag(false), 
                                                                             startTime(0), 
                                                                             stopTime(0),
                                                                             deltaTime(0),
                                                                             speed(0.0)
{
  pinMode(startPin, INPUT);
  pinMode(stopPin, INPUT);
  pinMode(indicatorPin, OUTPUT);
}

void SpeedMeter::startISR()
{
  instance->timesCrossedStartBarrier++;
  instance->startBarrierCrossedCallback();
}
void SpeedMeter::stopISR()
{
  instance->timesCrossedStopBarrier++;
  instance->stopBarrierCrossedCallback();
}
boolean SpeedMeter::barrierCrossed()
{
  boolean result = barrierFlag;
  if(barrierFlag){
    barrierFlag = false;
  }else{
    checkBarrier();
  }
  
  return result;
}
boolean SpeedMeter::checkBarrier()
{
  int start = digitalRead(startPin);
  int stop = digitalRead(stopPin);
  boolean result = (start>0 && stop>0);
  if(result){
    digitalWrite(indicatorPin, HIGH);
  }else{
    digitalWrite(indicatorPin, LOW);
  }
  return result;
}
void SpeedMeter::startBarrierCrossedCallback()
{
  startTime = micros();
}
void SpeedMeter::stopBarrierCrossedCallback()
{
  stopTime = micros();
  if(startTime>0){
    deltaTime = stopTime - startTime;
    startTime = 0;
    speed = (3600 * distance)/deltaTime;
    barrierFlag = true;
  }
}
int SpeedMeter::pinToInterrupt(int p)
{
  int result;
  switch(p){
    case 0:
      result = 2;
      
     
      break;
    case 1:
      result = 3;
      
      break;
    case 2:
      result = 1;
     
      break;
    case 3:
      result = 0;
      
      break;
    case 21:
      result = 2;
      break;
    case 20:
      result = 3;
      break;
    case 19:
      result = 4;
      break;
    case 18:
      result = 5;
      break;
  }
 
  return result;
}
void SpeedMeter::prepare()
{
  timesCrossedStartBarrier = 0;
  timesCrossedStopBarrier = 0;
  int startInterruptIndex = pinToInterrupt(startPin);
  int stopInterruptIndex = pinToInterrupt(stopPin);
  
  attachInterrupt(startInterruptIndex, &SpeedMeter::startISR, FALLING);
  attachInterrupt(stopInterruptIndex, &SpeedMeter::stopISR, FALLING);
}
void SpeedMeter::terminate()
{
  int startInterruptIndex = pinToInterrupt(startPin);
  int stopInterruptIndex = pinToInterrupt(stopPin);
  
  detachInterrupt(startInterruptIndex);
  detachInterrupt(stopInterruptIndex);
}
