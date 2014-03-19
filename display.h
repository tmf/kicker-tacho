#ifndef DISPLAY_H
#define SDISPLAY_H
class Display 
{
public:
  virtual void writeString(char *s) {
  };
  virtual void writeCharacter(char c, int pos) {
  };
  virtual void writeDouble(double d){};
  virtual void writeByte(byte b, int pos){};
  virtual void writeByte(byte b, int pos, int sendImmediately) {};
  virtual void update(unsigned long now);
  virtual void clear() {
  };
};
#endif

