
class WiflyShield : 
public SoftwareSerial {
private:
  int rxPin;
  int txPin;
  char ssid[64];
  char password[128];
public:
  WiflyShield(int,int, const char* wlan,const char* pass);
  void init();
  void connect(const char* host);
  char *command(const char* cmd);
  char *command(const char* cmd, boolean cr);
  char readChar();
};

WiflyShield::WiflyShield(int rx, int tx, const char *wlan, const char *pass) : 
SoftwareSerial(rx, tx), rxPin(rx), txPin(tx) 
{
  strcpy(ssid, wlan);
  strcpy(password, pass);
  init();
}

char *WiflyShield::command(const char* cmd){
  return command(cmd, true);
}
char *WiflyShield::command(const char* cmd, boolean cr){
  SoftwareSerial::print(cmd);
  if(cr){
    SoftwareSerial::print("\r");
  }
  Serial.print(cmd);
  Serial.print(": ");
  read();
}
char WiflyShield::readChar()
{
  if(SoftwareSerial::available()){
    char c = SoftwareSerial::read();
    Serial.print(c);
    return c;
  }
}
void WiflyShield::init() 
{
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  SoftwareSerial::begin(9600);
  delay(1000);

  command("$$$", false);
  delay(250);
  command("set uart baudrate 57600");
  command("save");
  command("reboot");

  delay(1000);
  SoftwareSerial::begin(57600);

  command("set wlan sswid ", false);
  command(ssid);

  delay(250);
  command("set wlan phrase ", false);
  command(password);
  command("save");
  command("reboot");
  delay(1000);

  command("set wlan join 1");
  command("set ip dhcp 1");
  command("join ", false);
  command(ssid);
  command("save");
  command("reboot");
  delay(1000);
  Serial.println("tschau");
  
}

void WiflyShield::connect(const char *host) 
{
  command("$$$", false);
  delay(250);
  command("open ", false);
  command(host);
}

