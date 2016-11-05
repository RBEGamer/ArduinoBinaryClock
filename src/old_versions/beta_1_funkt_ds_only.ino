/*
 * T(00-59)(00-59)(00-23)(1-7)(01-31)(01-12)(00-99) - T(sec)(min)(hour)(dayOfWeek)(dayOfMonth)(month)(year) -

*/
 #include <FastSPI_LED.h>
#define NUM_LEDS 25
struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
struct CRGB *leds;
#define PIN 7
#include "Wire.h"
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







 int s_z = 0;
int s_e = 0; 
byte binwert_s_z;
byte binwert_s_e;


//SEKUNDE
int szr = 111, szg = 0, szb = 0;
int ser = 0, seg = 111, seb = 0;
//AUSFABRE
int szrz = 10, szgz = 0, szbz = 0;
int serz = 0, segz = 10, sebz = 0;

//MINUTE
int mzr = 0, mzg = 0, mzb = 111;
int mer = 0, meg = 111, meb = 111;
//AUSFABRE
int mzrz = 0, mzgz = 0, mzbz = 10;
int merz = 0, megz = 10, mebz = 10;

//HOUR
int hzr = 111, hzg = 0, hzb = 111;
int her = 111, heg = 111, heb = 0;
//AUSFABRE
int hzrz = 10, hzgz = 0, hzbz = 10;
int herz = 10, hegz = 10, hebz = 0;

int nthe = 1; //ENABLE NIGHTMODE
int nth = 22, nthe = 6;
int nthr = 10, nthg = 0, nthb = 0;
int nthzr = 0, nthzg = 0, nthzb = 0;
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
 
// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers, Probably need to put in checks for valid numbers.
 
void setDateDs1307()                
{
   second = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48)); // Use of (byte) type casting and ascii math to achieve result.  
   minute = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
   hour  = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
   dayOfWeek = (byte) (Serial.read() - 48);
   dayOfMonth = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
   month = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
   year= (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
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
 

if(nthe == 1}{
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

if(nthstate == 1){
szr =nthr;
szg = nthg;
szb = nthb; 
szrz =nthzr;
szgz = nthzg;
szbz = nthzb;  
mzr =nthr;
mzg = nthg;
mzb = nthb; 
mzrz =nthzr;
mzgz = nthzg;
mzbz = nthzb;
hzr =nthr;
hzg = nthg;
hzb = nthb;  
hzrz =nthzr;
hzgz = nthzg;
hzbz = nthzb;

ser =nthr;
seg = nthg;
seb = nthb; 
serz =nthzr;
segz = nthzg;
sebz = nthzb;  
mer =nthr;
meg = nthg;
meb = nthb; 
merz =nthzr;
megz = nthzg;
mebz = nthzb;
her =nthr;
heg = nthg;
heb = nthb;  
herz =nthzr;
hegz = nthzg;
hebz = nthzb;
}

_value = second;
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


_value = minute;
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




_value = hour;
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
}
 
void loop() {
  
   getDateDs1307();
  
   if (Serial.available()) {      // Look for char in serial que and process if found
      command = Serial.read();
      if (command == 84 || command == 116) {      //If command = "Tt" Set Date
       setDateDs1307();
       getDateDs1307();
       Serial.println(" ");
      }
      else if (command == 82 || command == 114) {      //If command = "Rr" Read Date ... BBR
      
       Serial.println(" ");
      }
      else if (command == 81 || command == 113) {      //If command = "Qq" RTC1307 Memory Functions
        delay(100);     
        if (Serial.available()) {
         command = Serial.read(); 
         if (command == 49) {        //If command = "1" RTC1307 Initialize Memory - All Data will be set to 
                                       // 255 (0xff).  Therefore 255 or 0 will be an invalid value.  
           Wire.beginTransmission(DS1307_I2C_ADDRESS);   // 255 will be the init value and 0 will be considered 
                                                          // an error that occurs when the RTC is in Battery mode.
           I2C_WRITE(0x08); // Set the register pointer to be just past the date/time registers.
           for (i = 1; i <= 24; i++) {
               I2C_WRITE(0Xff);
              delay(10);
           }   
           Wire.endTransmission();
           Wire.beginTransmission(DS1307_I2C_ADDRESS);   
           I2C_WRITE(0x21); // Set the register pointer to 33 for second half of registers. Only 32 writes per connection allowed.
           for (i = 1; i <= 33; i++) {
               I2C_WRITE(0Xff);
              delay(10);
           }   
           Wire.endTransmission();
           getDateDs1307();
           Serial.println(": RTC1307 Initialized Memory");
         }
         else if (command == 50) {      //If command = "2" RTC1307 Memory Dump
          getDateDs1307();
          Serial.println(": RTC 1307 Dump Begin");
          Wire.beginTransmission(DS1307_I2C_ADDRESS);
          I2C_WRITE(zero);
          Wire.endTransmission();
          Wire.requestFrom(DS1307_I2C_ADDRESS, 32);
          for (i = 0; i <= 31; i++) {  //Register 0-31 - only 32 registers allowed per I2C connection
             test = I2C_READ();
             Serial.print(i);
             Serial.print(": ");
             Serial.print(test, DEC);
             Serial.print(" : ");
             Serial.println(test, HEX);
          }
          Wire.beginTransmission(DS1307_I2C_ADDRESS);
          I2C_WRITE(0x20);
          Wire.endTransmission();
          Wire.requestFrom(DS1307_I2C_ADDRESS, 32);  
          for (i = 32; i <= 63; i++) {         //Register 32-63 - only 32 registers allowed per I2C connection
             test = I2C_READ();
             Serial.print(i);
             Serial.print(": ");
             Serial.print(test, DEC);
             Serial.print(" : ");
             Serial.println(test, HEX);
          }
          Serial.println(" RTC1307 Dump end");
         } 
        }  
       }
      Serial.print("Command: ");
      Serial.println(command);     // Echo command CHAR in ascii that was sent
      }
      command = 0;                 // reset command 
      
      
      
      
      // reset command     
      delay(200);
    }








 
      
      
      
