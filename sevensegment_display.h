#ifndef SEVENSEGMENT_DISPLAY_H
#define SEVENSEGMENT_DISPLAY_H
#include "Arduino.h"
#include <string.h>
#include "display.h"
#include "charset_7seg.h"

class SevenSegmentDisplay : public Display 
{
  private:
    int displaySize;
    int dataPin;
    int latchPin;
    int clockPin;
    char buffer[256];
    long bufferSize;
    int bufferPosition;
    byte screenBuffer[32];
    int screenBufferSize;
    long scrollRate;
    long lastUpdate;
    boolean freeze;
  public:
    SevenSegmentDisplay(int dataPin, int clockPin, int latchPin, int displaySize);
    void writeString(char *s);
    void writeByte(byte b, int pos);
    void writeByte(byte b, int pos, int sendImmediately);
    void writeCharacter(char c, int pos);
    void writeCharacter(char c, int pos, int sendImmediately);
    void writeDouble(double d);
    void update(unsigned long now);
    void update(unsigned long now, boolean force);
    void clear();
    void flush();
};

SevenSegmentDisplay::SevenSegmentDisplay(int data_p, int clock_p, int latch_p, int disp_s) : dataPin(data_p), clockPin(clock_p), latchPin(latch_p), displaySize(disp_s)
{
  strcpy(buffer, "");
  bufferSize = 256;
  screenBufferSize = 32;
  bufferPosition = 0;
  scrollRate = 1000;
  lastUpdate = 0;
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}  
void SevenSegmentDisplay::writeString(char *s)
{
  if(strlen(s) < bufferSize){
    strcpy(buffer, s);
    freeze=false;
    bufferPosition = 0;
    lastUpdate = millis() - scrollRate;
    update(millis());
  }
}
void SevenSegmentDisplay::writeDouble(double d)
{
  double tmp=d;
  double divider = 10.0;
  for(int index=0;index<4;index++){
    int digit = tmp/divider;
    
    writeCharacter('0'+digit, index, false);
    if(divider<1.1 && divider > 0.9){
      screenBuffer[index] |= ledCharSet['.'];
    }
    tmp -= (digit * divider);
    divider /= 10.0;
    
  }
  writeCharacter(' ', 4, false);
  flush();
  freeze=true;
}
void SevenSegmentDisplay::writeCharacter(char c, int pos)
{
  writeCharacter(c, pos, true);
}
void SevenSegmentDisplay::writeByte(byte b, int pos){
  writeByte(b, pos, true);
}

void SevenSegmentDisplay::writeByte(byte b, int pos, int sendImmediately){
  screenBuffer[pos] = b;
  if(sendImmediately){
    flush();
  }
}
void SevenSegmentDisplay::writeCharacter(char c, int pos, int sendImmediately)
{
  freeze = false;
  screenBuffer[pos] = ledCharSet[c];
  if(sendImmediately){
    flush();
  }
}
void SevenSegmentDisplay::flush()
{
  if(displaySize > 0){
    boolean dot = false;
    int dotCharacterCount = 0;
    digitalWrite(latchPin, LOW);
    
    for(int index=displaySize-1; index>=0; index--){
      byte b = (screenBuffer[index]);
      if(b==ledCharSet['.']){
        dot = true;
      }else{
        if(dot){
          b |= ledCharSet['.'];
          dotCharacterCount++;
          dot = false;
        }
        b = ~b;
        shiftOut(dataPin, clockPin, LSBFIRST, b);
      }
    }
    byte space = ledCharSet[' '];
    space = ~space;
    for(int index=0;index<dotCharacterCount;index++){
      shiftOut(dataPin, clockPin, LSBFIRST, space);
    }
    digitalWrite(latchPin, HIGH);
  }
}
void SevenSegmentDisplay::update(unsigned long now)
{
  update(now, false);
}
void SevenSegmentDisplay::update(unsigned long now, boolean force)
{
  if(force || (!freeze && now >= lastUpdate + scrollRate)) {
    for(int index=0;index<displaySize;index++){
      char c;
      if((bufferPosition + index)<strlen(buffer)){
        c = buffer[bufferPosition + index];
      }else{
        c = ' ';
      }
      writeCharacter(c, index, false);
      
    }
    flush();
    
    if(bufferPosition + displaySize < strlen(buffer)){
      bufferPosition++;
    }else{
      bufferPosition = 0;
    }
    lastUpdate = now;
  }
}
void SevenSegmentDisplay::clear()
{
  for(int index=0;index<bufferSize;index++){
    buffer[index] = ' ';
  }
  buffer[bufferSize-1] = '\0';
  bufferPosition = 0;
  for(int index=0;index<displaySize;index++){
    writeCharacter(buffer[index], index, false);
  }
  lastUpdate = millis() - scrollRate;
  flush();
}

#endif
