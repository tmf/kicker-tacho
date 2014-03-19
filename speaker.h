/*
 * Play example using Time3 library.
 *
 * The sample must be raw 8bits 8KHz, please read README.md.
 *
 *  Juan J. Martinez <jjm@usebox.net>
 *  https://github.com/reidrac/play-pcm-example
 */

#include <avr/pgmspace.h>
#include "TimerOne.h"



class Speaker 
{
  static void playISR();
  static Speaker * instance;
  
  private:
    volatile unsigned char *play_sample;
    volatile unsigned short play_sample_length;
    volatile unsigned short play_sample_ptr;
    volatile int speakerPin;
  
  public:
    Speaker(int pin);
    void playCallback();
    void play(const unsigned char *sample, const unsigned short length);
    void stop();
};

Speaker::Speaker(int pin) : speakerPin(pin) 
{ 
  pinMode(speakerPin, OUTPUT); 
  instance = this;
}

void Speaker::playISR(){
  instance->playCallback();
}
// for use by ISR glue routines
Speaker * Speaker::instance;
void Speaker::playCallback(void)
{
  if(play_sample_length)
  {
    Timer1.setPwmDuty(speakerPin, pgm_read_byte(&play_sample[play_sample_ptr]));
    play_sample_ptr++;

    if(play_sample_ptr == play_sample_length)
    {
      play_sample_length = 0;
      Timer1.setPwmDuty(speakerPin, 0);
    }
  }
}


/* for Teensy 2.0: pin 9 */
void Speaker::play(const unsigned char *sample, const unsigned short length)
{
  play_sample_length = 0;
  

  

  // 8KHz are 125 microsenconds
  Timer1.initialize(125);
  Timer1.attachInterrupt(playISR);
  Timer1.pwm(speakerPin, HIGH);
  noInterrupts();
  play_sample = (unsigned char *)sample;
  play_sample_ptr = 0;
  play_sample_length = length;
  interrupts();
}



void Speaker::stop(){
   noInterrupts();
   play_sample_length = 0;
   Timer1.setPwmDuty(speakerPin, 0);
   Timer1.stop();
   Timer1.detachInterrupt();
   interrupts();
}

