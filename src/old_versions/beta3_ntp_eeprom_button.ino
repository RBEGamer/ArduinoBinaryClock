#include <FastSPI_LED.h>
#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "Wire.h"
#include <EEPROM.h>

#define NUM_LEDS 25
struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
struct CRGB *leds;
#define PIN 7

#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
// Arduino version compatibility Pre-Compiler Directives
#if defined(ARDUINO) && ARDUINO >= 100   // Arduino v1.0 and newer
  #define I2C_WRITE Wire.write 
  #define I2C_READ Wire.read
#else                                   // Arduino Prior to v1.0 
  #define I2C_WRITE Wire.send 
  #define I2C_READ Wire.receive
#endif
// Global Variables
int command = 0;       // This is the command char, in ascii form, sent from the serial port     
int i;
long previousMillis = 0;        // will store last time Temp was updated
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
byte test;
byte zero;
int _value;


//ETHERNET CONF
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
unsigned int localPort = 8888;      // local port to listen for UDP packets
//IPAddress timeServer(134, 169, 9, 108); // time-a.timefreq.bldrdoc.gov NTP server
// IPAddress timeServer(131, 188, 3, 221); // time-b.timefreq.bldrdoc.gov NTP server
 IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov NTP server
const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 
// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
//hir ist die zeit die vom server gehot wurde
int ntph = 0;
int ntpm = 0;
int ntps = 0;

unsigned int zeitverschiebung = 2; //zeit um 2 stunden nach vorne verschieben
const int sw1 = 6;
const int sw2 = 5;
const int sw3 = 4;
int prev_state_button = 0;

//für die splittung
int s_z = 0;
int s_e = 0; 
byte binwert_s_z;
byte binwert_s_e;

//SEKUNDE
int szr = 70, szg = 0, szb = 0;
int ser = 0, seg = 70, seb = 0;
//AUSFABRE
int szrz = 10, szgz = 0, szbz = 0;
int serz = 0, segz = 10, sebz = 0;

//MINUTE
int mzr = 0, mzg = 0, mzb = 70;
int mer = 0, meg = 70, meb = 70;
//AUSFABRE
int mzrz = 0, mzgz = 0, mzbz = 10;
int merz = 0, megz = 10, mebz = 10;

//HOUR
int hzr = 70, hzg = 0, hzb = 70;
int her = 70, heg = 70, heb = 0;
//AUSFABRE
int hzrz = 10, hzgz = 0, hzbz = 10;
int herz = 10, hegz = 10, hebz = 0;


int nthen = 1; //ENABLE NIGHTMODE
int nth = 22, nthe = 6;
int nthr = 0, nthg = 0, nthb = 8;
int nthzr = 0, nthzg = 4, nthzb = 0;
int nthstate = 0;
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
 
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}
 

 
void setDateDs1307()                
{
   second = ntps;
   minute = ntpm;
   //hier die zeitverschiebung beachten
   hour  = ntph+zeitverschiebung;
   if(hour == 24){
   hour = 0;
   }if(hour > 24){
   
   hour = hour -24;
   }
   
   
   
   dayOfWeek = 1;
   dayOfMonth = 1;
   month = 1;
   year= 1;
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   I2C_WRITE(zero);
   I2C_WRITE(decToBcd(second) & 0x7f);    // 0 to bit 7 starts the clock
   I2C_WRITE(decToBcd(minute));
   I2C_WRITE(decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   I2C_WRITE(decToBcd(dayOfWeek));
   I2C_WRITE(decToBcd(dayOfMonth));
   I2C_WRITE(decToBcd(month));
   I2C_WRITE(decToBcd(year));
   Wire.endTransmission();
   
   
   
  
}
 
// Gets the date and time from the ds1307 and prints result
void getDateDs1307()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  I2C_WRITE(zero);
  Wire.endTransmission();
 
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
 
  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(I2C_READ() & 0x7f);
  minute     = bcdToDec(I2C_READ());
  hour       = bcdToDec(I2C_READ() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(I2C_READ());
  dayOfMonth = bcdToDec(I2C_READ());
  month      = bcdToDec(I2C_READ());
  year       = bcdToDec(I2C_READ());
 

if(nthen == 1){
//LIEGT DIE ZEIT ZWISCHEN DEN BEIDEN GERENDZEN
if(nth > nthe){
//WENN ANFANG > ALS ENDE
if(hour <= nth && hour <= nthe){
nthstate = 1;
}else if(hour >= nth && hour >= nthe){
nthstate = 1;
}else{
nthstate = 0;
}
}else if (nth < nthe){
  //WENN ANFANG > ALS ENDE
if(hour <= nth && hour >= nthe){
nthstate = 1;
}else if(hour >= nth && hour <= nthe){
nthstate = 1;
}else{
nthstate = 0;
}
}else if( nth == nthe){
  if(hour == nth || hour == nthe){
  nthstate = 1;
  }else{
  nthstate = 0;
  }
}else{
nthstate = 0;
}
}else{
nthstate == 0;
}

if(nthstate == 1 && nthen == 1){
szr =nthr;szg = nthg;szb = nthb; szrz =nthzr;szgz = nthzg;szbz = nthzb;  mzr =nthr;mzg = nthg;mzb = nthb; mzrz =nthzr;mzgz = nthzg;mzbz = nthzb;hzr =nthr;hzg = nthg;hzb = nthb;  hzrz =nthzr;hzgz = nthzg;hzbz = nthzb;
ser =nthr;seg = nthg;seb = nthb; serz =nthzr;segz = nthzg;sebz = nthzb;  mer =nthr;meg = nthg;meb = nthb; merz =nthzr;megz = nthzg;mebz = nthzb;her =nthr;heg = nthg;heb = nthb;  herz =nthzr;hegz = nthzg;hebz = nthzb;
}else{
}





  delay(130);

display_time(hour, minute, second);


  
}
 
 
 
 
 
 void display_time(int _hour, int _minute, int _second){
 
 _value = _second;
 s_z = (_value % 100) / 10;
 s_e = _value % 10;
 binwert_s_z = byte(s_z);
 binwert_s_e = byte(s_e);


      //SEKUNDEN ZEHNER
   if(bitRead(binwert_s_z,0) == 1){
   leds[7].r = szr;leds[7].g = szg;leds[7].b = szb;
   }else{
      leds[7].r = szrz;leds[7].g = szgz;leds[7].b = szbz; 
   }  
   if(bitRead(binwert_s_z,1) == 1){
    leds[6].r = szr;leds[6].g = szg;leds[6].b = szb;
   }else{
      leds[6].r = szrz;leds[6].g = szgz;leds[6].b = szbz; 
   } 
   if(bitRead(binwert_s_z,2) == 1){
    leds[5].r = szr;leds[5].g = szg;leds[5].b = szb;
   }else{
      leds[5].r = szrz;leds[5].g = szgz;leds[5].b = szbz; 
   }  
   if(bitRead(binwert_s_z,3) == 1){
    leds[4].r = szr;leds[4].g = szg;leds[4].b = szb;
   }else{
     leds[4].r = szrz;leds[4].g = szgz;leds[4].b = szbz; 
   }
   //SEKUNDEN EINER
   if(bitRead(binwert_s_e,0) == 1){
   leds[0].r = ser;leds[0].g = seg;leds[0].b = seb; 
   }else{
    leds[0].r = serz;leds[0].g = segz;leds[0].b = sebz; 
   }  
   if(bitRead(binwert_s_e,1) == 1){
   leds[1].r = ser;leds[1].g = seg;leds[1].b = seb; 
   }else{
      leds[1].r = serz;leds[1].g = segz;leds[1].b = sebz; 
   } 
   if(bitRead(binwert_s_e,2) == 1){
  leds[2].r = ser;leds[2].g = seg;leds[2].b = seb; 
   }else{
      leds[2].r = serz;leds[2].g = segz;leds[2].b = sebz; 
   }  
   if(bitRead(binwert_s_e,3) == 1){
   leds[3].r = ser;leds[3].g = seg;leds[3].b = seb; 
   }else{
      leds[3].r = serz;leds[3].g = segz;leds[3].b = sebz; 
   }


_value = _minute;
 s_z = (_value % 100) / 10;
 s_e = _value % 10;
 binwert_s_z = byte(s_z);
 binwert_s_e = byte(s_e);


      //MINUTEN ZEHNER
   if(bitRead(binwert_s_z,0) == 1){
   leds[15].r = mzr;leds[15].g = mzg;leds[15].b = mzb;
   }else{
      leds[15].r = mzrz;leds[15].g = mzgz;leds[15].b = mzbz; 
   }  
   if(bitRead(binwert_s_z,1) == 1){
    leds[14].r = mzr;leds[14].g = mzg;leds[14].b = mzb;
   }else{
      leds[14].r = mzrz;leds[14].g = mzgz;leds[14].b = mzbz; 
   } 
   if(bitRead(binwert_s_z,2) == 1){
    leds[13].r = mzr;leds[13].g = mzg;leds[13].b = mzb;
   }else{
      leds[13].r = mzrz;leds[13].g = mzgz;leds[13].b = mzbz; 
   }  
   if(bitRead(binwert_s_z,3) == 1){
    leds[12].r = mzr;leds[12].g = mzg;leds[12].b = mzb;
   }else{
     leds[12].r = mzrz;leds[12].g = mzgz;leds[12].b = mzbz; 
   }
   
   //MINUTE EINER
   if(bitRead(binwert_s_e,0) == 1){
   leds[8].r = mer;leds[8].g = meg;leds[8].b = meb; 
   }else{
    leds[8].r = merz;leds[8].g = megz;leds[8].b = mebz; 
   }  
   if(bitRead(binwert_s_e,1) == 1){
   leds[9].r = mer;leds[9].g = meg;leds[9].b = meb; 
   }else{
      leds[9].r = merz;leds[9].g = megz;leds[9].b = mebz; 
   } 
   if(bitRead(binwert_s_e,2) == 1){
  leds[10].r = mer;leds[10].g = meg;leds[10].b = meb; 
   }else{
      leds[10].r = merz;leds[10].g = megz;leds[10].b = mebz; 
   }  
   if(bitRead(binwert_s_e,3) == 1){
   leds[11].r = mer;leds[11].g = meg;leds[11].b = meb; 
   }else{
      leds[11].r = merz;leds[11].g = megz;leds[11].b = mebz; 
   }




_value = _hour;
 s_z = (_value % 100) / 10;
 s_e = _value % 10;
 binwert_s_z = byte(s_z);
 binwert_s_e = byte(s_e);


      //HOUR ZEHNER
   if(bitRead(binwert_s_z,3) == 1){
   leds[20].r = hzr;leds[20].g = hzg;leds[20].b = hzb;
   }else{
      leds[20].r = hzrz;leds[20].g = hzgz;leds[20].b = hzbz; 
   }  
   if(bitRead(binwert_s_z,2) == 1){
    leds[21].r = hzr;leds[21].g = hzg;leds[21].b = hzb;
   }else{
      leds[21].r = hzrz;leds[21].g = hzgz;leds[21].b = hzbz; 
   } 
   if(bitRead(binwert_s_z,1) == 1){
    leds[22].r = hzr;leds[22].g = hzg;leds[22].b = hzb;
   }else{
      leds[22].r = hzrz;leds[22].g = hzgz;leds[22].b = hzbz; 
   }  
   if(bitRead(binwert_s_z,0) == 1){
    leds[23].r = hzr;leds[23].g = hzg;leds[23].b = hzb;
   }else{
     leds[23].r = hzrz;leds[23].g = hzgz;leds[23].b = hzbz; 
   }
 
 
   //HOUR EINER
   if(bitRead(binwert_s_e,3) == 1){
   leds[19].r = her;leds[19].g = heg;leds[19].b = heb; 
   }else{
    leds[19].r = herz;leds[19].g = hegz;leds[19].b = hebz; 
   }  
   if(bitRead(binwert_s_e,2) == 1){
   leds[18].r = her;leds[18].g = heg;leds[18].b = heb; 
   }else{
      leds[18].r = herz;leds[18].g = hegz;leds[18].b = hebz; 
   } 
   if(bitRead(binwert_s_e,1) == 1){
  leds[17].r = her;leds[17].g = heg;leds[17].b = heb; 
   }else{
      leds[17].r = herz;leds[17].g = hegz;leds[17].b = hebz; 
   }  
   if(bitRead(binwert_s_e,0) == 1){
   leds[16].r = her;leds[16].g = heg;leds[16].b = heb; 
   }else{
      leds[16].r = herz;leds[16].g = hegz;leds[16].b = hebz; 
   }





  FastSPI_LED.show();
 }
 
 
 
void setup() {
   pinMode(sw1, INPUT);
    pinMode(sw2, INPUT);
     pinMode(sw3, INPUT);
     
     
     nth = EEPROM.read(0);
     nthe = EEPROM.read(1);
     nthen = EEPROM.read(2);
  Wire.begin();
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  FastSPI_LED.setPin(PIN);
  FastSPI_LED.setDataRate(1);
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  Serial.begin(9600); 
  zero=0x00;
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
   // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  Udp.begin(localPort);
  
  
  


}
 
void loop() {
 
//SEKUNDE
 szr = 70; szg = 0; szb = 0;
 ser = 0; seg = 70; seb = 0;
//AUSFABRE
 szrz = 10; szgz = 0; szbz = 0;
 serz = 0; segz = 10; sebz = 0;

//MINUTE
 mzr = 0; mzg = 0; mzb = 70;
 mer = 0; meg = 70; meb = 70;
//AUSFABRE
 mzrz = 0; mzgz = 0; mzbz = 10;
 merz = 0; megz = 10; mebz = 10;

//HOUR
 hzr = 70; hzg = 0; hzb = 70;
 her = 70; heg = 70; heb = 0;
//AUSFABRE
 hzrz = 10; hzgz = 0; hzbz = 10;
 herz = 10; hegz = 10; hebz = 0;


   getDateDs1307();
  delay(200);

   


if(digitalRead(sw1) == HIGH&& digitalRead(sw2) == LOW && digitalRead(sw3) == LOW && prev_state_button == 0){ 
Serial.println("ENTER EDIT MODE");
int buff_m = EEPROM.read(2);
int buff_h = EEPROM.read(0);
int buff_s = EEPROM.read(1);
int prev_state_h = 0;
int prev_state_m = 0;
int prev_state_s = 0;

while (digitalRead(sw1) == HIGH ){ 
 if(digitalRead(sw2) == HIGH && prev_state_h == 0 && digitalRead(sw3) == LOW){
   prev_state_h = 1;  
   if(buff_h > 23){
   buff_h = 0;
   }else{
   buff_h++;
   }      
 }else{
 prev_state_h = 0;
 }
   if(digitalRead(sw3) == HIGH && prev_state_m == 0 && digitalRead(sw2) == HIGH){
     prev_state_m = 1;
   if(buff_m == 0){
   buff_m = 1;
   }else{
   buff_m = 0;
   }      
}else{
prev_state_m = 0;
}
   if(digitalRead(sw3) == HIGH && digitalRead(sw2) == LOW  && prev_state_s == 0){
     prev_state_s = 1;
   if(buff_s > 23){
   buff_s = 0;
   }else{
   buff_s++;
   }      
}else{
prev_state_s = 0;
}

 
display_time(buff_h,buff_m,buff_s);
delay(200);
}//END WHILE

//WERTE ÜBERNHEMEN
EEPROM.write(0,buff_h); //NT
EEPROM.write(1,buff_s); //NTE
EEPROM.write(2,buff_m); //NTEN
nth = buff_h;
nthe = buff_s;
nthen = buff_m;
setup();
}else if( digitalRead(sw1) == LOW && digitalRead(sw2) == LOW && digitalRead(sw3) == HIGH && prev_state_button == 0){
prev_state_button = 1;
Serial.println("START NTP REQUEST");
  get_ntp_time(); 
   setDateDs1307();
   delay(1000);
}else{
prev_state_button = 0;
} 

//END IF HIGH


}


// send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address)
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}
void get_ntp_time(){
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  delay(1000);  
  if ( Udp.parsePacket() ) {   
    Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);    
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    const unsigned long seventyYears = 2208988800UL;       
    unsigned long epoch = secsSince1900 - seventyYears;                           
    ntph=(epoch  % 86400L) / 3600; 
    ntpm=(epoch  % 3600) / 60;
    ntps = epoch %60;
  }
}





 
      
      
      
