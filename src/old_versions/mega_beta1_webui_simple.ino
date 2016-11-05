/*
TASTENBELEGUNG : 
Taster 3 kurz drücken : get server time (internet muss vorhanden sein)
Taster 2 kurz drücken: änder LED Mode
Taster 1+2 gedrückt halten: mit Taster 2 die zeitzone immer +1h ändern. dann Taster 1+2 loslassen und taster 3 kurz drücken um diese zu übernhemen und get server time (internet muss vorhanden sein)
Taster 1 gedrückt halten : nighmode ändern : Taster 2 kurz drücken : nighmode an/aus
                                              Taster 1 : nighmode anfangsstunde ändern immer +1h
                                             Taster 3 : nighmode endstunde ändern immer +1h





*/









#include <FastSPI_LED.h>
#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "Wire.h"
#include <EEPROM.h>

#define PIN 7 //PIN DER LEDS
 IPAddress timeServer(132, 163, 4, 103); // HIER DIE NTC SERVER ADRESSE EINTRAGEN
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //MACADRESSE EINTRAGEN
const int step_width = 1; //SCHNELLIGKEIT DES FADES 1
 int max_zero_bright = 5; //HELLIGKEIT LED AUS
const int main_delay = 100; //SYSTEM DELAY NICHT SCHNELLER ALS 100
 int max_bright = 70; //HELLIGKEIT LED AN
const int sw1 = 6; //SCHALTER 1 PIN
const int sw2 = 5; //SCHALTER 2 PIN
const int sw3 = 4; //SCHALTER 3 PIN

#define NUM_LEDS 24
struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
struct CRGB *leds;
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
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year, _hour;
byte test;
byte zero;
int _value;
unsigned int localPort = 8888;      // local port to listen for UDP packets
const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 
// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
//hir ist die zeit die vom server gehot wurde
int ntph = 0;
int ntpm = 0;
int ntps = 0;
int ether_begin_state = 0; //DAMIT NUR EINEMAL DIE ETHERNETLEITuNG INIT
unsigned int zeitverschiebung = 0; //zeit um 2 stunden nach vorne verschieben
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

int nthr = 0, nthg = 2, nthb = 0;
int nthzr = 0, nthzg = 0, nthzb = 1;

int nthstate = 0;
int stop_effect = 0; //wenn 1 ist dann werden alle effectschleifen abgebrochen
const int maxmode = 4;
 int ledmode = 0;
 int redIntensity = 0;
int greenIntensity = 0;
int blueIntensity = 0;
int _redIntensity;
int _greenIntensity;
int _blueIntensity;
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}
 



 
// Gets the date and time from the ds1307 and prints result
void getDateDs1307()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  I2C_WRITE(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
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
 
 
 
 
 
 
 
 const int use_webui = 1;
 const int use_ethernet = 1; //SOLL ETHERNET BENUTZT WERDEN
 const int wait_for_DHCP = 0;
 IPAddress ip(192,168,178, 199);
IPAddress gateway(192,168,178, 1);
IPAddress subnet(255, 255, 255, 0);
int webui_port = 80;
String readString;
EthernetServer webui_server(webui_port);

void setup() {
      Serial.println("");
  //EEPROM.write(6,80);
  //EEPROM.write(5,70);
  //EEPROM.write(4,10);
  
   pinMode(sw1, INPUT);
    pinMode(sw2, INPUT);
     pinMode(sw3, INPUT);  
     ledmode = 1;
     stop_effect = 0;  

     
     
     nth = EEPROM.read(0);
     nthe = EEPROM.read(1);
     nthen = EEPROM.read(2);
     zeitverschiebung = EEPROM.read(3);
     max_zero_bright =  EEPROM.read(4);
     max_bright =  EEPROM.read(5);
     webui_port = EEPROM.read(6);
     
     
  Wire.begin();
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  FastSPI_LED.setPin(PIN);
  FastSPI_LED.setDataRate(1);
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  zero=0x00;
  Serial.begin(9600);
  
  
  if(use_ethernet == 1){
    Serial.println("Trying to get an IP address using DHCP");
   // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
 if(wait_for_DHCP == 1){
  for(;;)
      ;
 }else{
    Serial.println("Failed to configure Ethernet using DHCP : USE ALTERNATIVE IP CONFIGURATION");
  Ethernet.begin(mac, ip, gateway, subnet);
 }
   
   
   
  }
  
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print("."); 
  }
  
  Serial.println("START UDP");
  Udp.begin(localPort);
  if(use_webui == 1){
    Serial.print("START WEBSERVER AT PORT : ");Serial.print(webui_port);
 webui_server.begin();
  }

 
  }
  
  
  
}
 
 void webui(){
if(use_webui == 1){
  EthernetClient webui_client = webui_server.available();
  if (webui_client) {
    while (webui_client.connected()) {
      if (webui_client.available()) {
        char c = webui_client.read();
 
        //read char by char HTTP request
        if (readString.length() < 100) {
 
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
 
        //if HTTP request has ended
        if (c == '\n') {
 
          ///////////////
          Serial.println(readString); //print to serial monitor for debuging
 
          webui_client.println("HTTP/1.1 200 OK"); //send new page
          webui_client.println("Content-Type: text/html");
          webui_client.println();
 
          webui_client.println("<HTML>");
          webui_client.println("<HEAD>");
          webui_client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          webui_client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          webui_client.println("<link rel='stylesheet' type='text/css' href='http://homeautocss.net84.net/a.css' />");
          webui_client.println("<TITLE>CLOCK CONTROL</TITLE>");
          webui_client.println("</HEAD>");
          webui_client.println("<BODY>");
          webui_client.println("<H1>CLOCK CONTROL</H1>");
          webui_client.println("<hr />");
          webui_client.println("<br />");
         
webui_client.println("<H2>SET CURRENT MODE</H2>");
if(ledmode != 0){  webui_client.println(" <a href=\'/?mode1\'\' color='green'><font color='#FF0000'>MODE 1</font></a>");}else{webui_client.println(" <a href=\'/?mode1\'\'><font color='#00FF00'>MODE 1</font></a>");}
if(ledmode != 1){  webui_client.println(" <a href=\'/?mode2\'\' color='green'><font color='#FF0000'>MODE 2</font></a>");}else{webui_client.println(" <a href=\'/?mode2\'\'><font color='#00FF00'>MODE 2</font></a>");}
if(ledmode != 2){  webui_client.println(" <a href=\'/?mode3\'\' color='green'><font color='#FF0000'>MODE 3</font></a>");}else{webui_client.println(" <a href=\'/?mode3\'\'><font color='#00FF00'>MODE 3</font></a>");}
if(ledmode != 3){  webui_client.println(" <a href=\'/?mode4\'\' color='green'><font color='#FF0000'>MODE 4</font></a>");}else{webui_client.println(" <a href=\'/?mode4\'\'><font color='#00FF00'>MODE 4</font></a>");}
if(ledmode != 4){  webui_client.println(" <a href=\'/?mode5\'\' color='green'><font color='#FF0000'>MODE 5</font></a>");}else{webui_client.println(" <a href=\'/?mode5\'\'><font color='#00FF00'>MODE 5</font></a>");}
webui_client.println("<br /><br />");      
 webui_client.println("<H2>TOGGLE NIGHMODE</H2>");
 
 if(nthen == 1){  webui_client.println(" <a href=\'/?nthen_toggle\'\' color='green'><font color='#FF0000'>MODE 4</font></a>");}else{webui_client.println(" <a href=\'/?nthen_toggle\'\'><font color='#00FF00'>MODE 4</font></a>");}

 
 
          webui_client.println("</BODY>");
          webui_client.println("</HTML>");
          delay(1);
          webui_client.stop();
 
       
          if(readString.indexOf("?mode1") >0){switch_ledmode(0);}          
          if(readString.indexOf("?mode2") >0){switch_ledmode(1);}          
          if(readString.indexOf("?mode3") >0){switch_ledmode(2);}
           if(readString.indexOf("?mode4") >0){switch_ledmode(3);}
           if(readString.indexOf("?mode5") >0){switch_ledmode(4);}
           if(readString.indexOf("?nthen_toggle") >0){if(nthen == 1){nthen = 0;}else{nthen = 1;}save_settings();}
             
             
           
          //clearing string for next read
          readString="";
 
        }
      }
    }
  }
 }
}


void switch_ledmode(int mode){
  Serial.print("switch LED-Mode from  ");Serial.print(ledmode);Serial.print(" to  ");Serial.print(mode);
ledmode = mode;
stop_effect = 1;

}


void save_settings(){
 Serial.println("SAFE ALL SETTINGS TO EEPROM"); 
EEPROM.write(0,nth); //NT
EEPROM.write(1,nthe); //NT
EEPROM.write(2,nthen); //NT
EEPROM.write(3,zeitverschiebung); //NT
EEPROM.write(4,max_zero_bright); //NT
EEPROM.write(5,max_bright); //NT
EEPROM.write(6,webui_port); //NT

}

void loop() {

  if(ledmode == 0){
 szr = 0; szg = 0; szb = 0;
 ser = 0; seg = 0; seb = 0;
//AUSFABRE
 szrz = 0; szgz = 0; szbz = 0;
 serz = 0; segz = 0; sebz = 0;

//MINUTE
 mzr = 0; mzg = 0; mzb = 0;
 mer = 0; meg = 0; meb = 0;
//AUSFABRE
 mzrz = 0; mzgz = 0; mzbz = 0;
 merz = 0; megz = 0; mebz = 0;

//HOUR
 hzr = 0; hzg = 0; hzb = 0;
 her = 0; heg = 0; heb = 0;
//AUSFABRE
 hzrz = 0; hzgz = 0; hzbz = 0;
 herz = 0; hegz = 0; hebz = 0;
   getDateDs1307();
          button();
            webui();
  delay(main_delay);   
}else if(ledmode == 1){
 szr = max_bright; szg = 0; szb = 0;
 ser = 0; seg = max_bright; seb = 0;
//AUSFABRE
 szrz = max_zero_bright; szgz = 0; szbz = 0;
 serz = 0; segz = max_zero_bright; sebz = 0;

//MINUTE
 mzr = 0; mzg = 0; mzb =max_bright;
 mer = 0; meg = max_bright; meb = max_bright;
//AUSFABRE
 mzrz = 0; mzgz = 0; mzbz = max_zero_bright;
 merz = 0; megz = max_zero_bright; mebz = max_zero_bright;

//HOUR
 hzr = max_bright; hzg = 0; hzb = max_bright;
 her = max_bright; heg = max_bright; heb = 0;
//AUSFABRE
 hzrz = max_zero_bright; hzgz = 0; hzbz = max_zero_bright;
 herz = max_zero_bright; hegz = max_zero_bright; hebz = 0;
 getDateDs1307();
          button();
            webui();
  delay(main_delay);
}else if(ledmode == 4){
 szr = max_bright; szg = 0; szb = 0;
 ser = max_bright; seg = 0; seb = 0;
//AUSFABRE
 szrz = max_zero_bright; szgz = 0; szbz = 0;
 serz = max_zero_bright; segz = 0; sebz = 0;

//MINUTE
 mzr = 0; mzg = max_bright; mzb =0;
 mer = 0; meg = max_bright; meb = 0;
//AUSFABRE
 mzrz = 0; mzgz = max_zero_bright; mzbz = 0;
 merz = 0; megz = max_zero_bright; mebz = 0;

//HOUR
 hzr = 0; hzg = 0; hzb = max_bright;
 her = 0; heg = 0; heb = max_bright;
//AUSFABRE
 hzrz = 0; hzgz = 0; hzbz = max_zero_bright;
 herz = 0; hegz = 0; hebz = max_zero_bright;
 getDateDs1307();
          button();
            webui();
  delay(main_delay);
}else if( ledmode == 2){
  for (greenIntensity = 0; greenIntensity <= max_bright; greenIntensity+=step_width) {
        redIntensity = max_bright-greenIntensity;        
        ser = redIntensity; seg = 0; seb = greenIntensity;
        szr = redIntensity; szg = greenIntensity; szb = 0;        
         mzr = 0; mzg = redIntensity; mzb = greenIntensity;
         mer = greenIntensity; meg = redIntensity; meb = 0;         
          hzr = greenIntensity; hzg = redIntensity; hzb = 0;
          her = redIntensity; heg = 0; heb = greenIntensity;       
         _redIntensity = redIntensity;
         _greenIntensity = greenIntensity;       
        if(_greenIntensity >= max_zero_bright){_greenIntensity = max_zero_bright;}
         if(_redIntensity >= max_zero_bright){_redIntensity = max_zero_bright;}       
          szrz = _redIntensity; szgz = _greenIntensity; szbz = 0;
         serz = _redIntensity; segz = 0; sebz = _greenIntensity;     
         mzrz = 0; mzgz = _redIntensity; mzbz = _greenIntensity;
         merz = _greenIntensity; megz = _redIntensity; mebz = 0;       
          hzrz = _greenIntensity; hzgz = _redIntensity; hzbz = 0;
          herz = _redIntensity; hegz = 0; hebz = _greenIntensity;      
        if(stop_effect == 1){stop_effect = 0;break;}
       getDateDs1307();     
       button();
         webui();
        delay(main_delay);
  } 
  for (blueIntensity = 0; blueIntensity <= max_bright; blueIntensity+=step_width) {
        greenIntensity = max_bright-blueIntensity;
         ser = blueIntensity; seg = 0; seb = greenIntensity;
         szr = 0; szg = greenIntensity; szb = blueIntensity;      
         mzr = blueIntensity; mzg = greenIntensity; mzb = 0;
         mer = greenIntensity; meg = 0; meb = blueIntensity;       
          hzr = greenIntensity; hzg = 0; hzb = blueIntensity;
          her = 0; heg = blueIntensity; heb = greenIntensity;        
      _greenIntensity =greenIntensity;
      _blueIntensity =blueIntensity;   
         if(_greenIntensity >= max_zero_bright){_greenIntensity = max_zero_bright;}
         if(_blueIntensity >= max_zero_bright){_blueIntensity = max_zero_bright;}    
        szrz = 0; szgz = _greenIntensity; szbz = _blueIntensity;
        serz = _blueIntensity; segz = 0; sebz = _greenIntensity;     
         mzrz = _blueIntensity; mzgz = _greenIntensity; mzbz = 0;
         merz = _greenIntensity; megz = 0; mebz = _blueIntensity;     
         hzrz = _greenIntensity; hzgz = 0; hzbz = _blueIntensity;
          herz = 0; hegz = _blueIntensity; hebz = _greenIntensity;        
          if(stop_effect == 1){stop_effect = 0;break;}
        getDateDs1307();
        button();
          webui();
        delay(main_delay);
  }

  // Cycle cycle from blue through to red
  // (In this loop we move from 100% blue, 0% red to 0% blue, 100% red)    
  for (redIntensity = 0; redIntensity <= max_bright; redIntensity+=step_width) {
        blueIntensity = max_bright-redIntensity;        
        ser = 0; seg = blueIntensity; seb = redIntensity;
        szr = redIntensity; szg = 0; szb = blueIntensity;        
         mzr = redIntensity; mzg = blueIntensity; mzb = 0;
         mer = 0; meg = redIntensity; meb = blueIntensity;                 
          hzr = 0; hzg = redIntensity; hzb = blueIntensity;
          her = 0; heg = blueIntensity; heb = redIntensity;         
         _redIntensity = redIntensity;
          _blueIntensity =blueIntensity;         
         if(_redIntensity >= max_zero_bright){_redIntensity = max_zero_bright;}
         if(_blueIntensity >= max_zero_bright){_blueIntensity = max_zero_bright;}      
        szrz = _redIntensity; szgz = 0; szbz = _blueIntensity;
        serz = 0; segz = _blueIntensity; sebz = _redIntensity;       
         mzrz = _redIntensity; mzgz = _blueIntensity; mzbz = 0;
         merz = 0; megz = _redIntensity; mebz = _blueIntensity;        
         hzrz = 0; hzgz = _redIntensity; hzbz = _blueIntensity;
          herz = 0; hegz = _blueIntensity; hebz = _redIntensity;        
         if(stop_effect == 1){stop_effect = 0;break;}
        getDateDs1307();
        button();
          webui();
        delay(main_delay);
  }
  
  
}else if(ledmode == 3){
  for (greenIntensity = 0; greenIntensity <= max_bright; greenIntensity+=step_width) {
        redIntensity = 255-greenIntensity;       
        szr =redIntensity; szg = greenIntensity; szb = 0;
        ser = redIntensity; seg = greenIntensity; seb = 0;
        mzr = redIntensity; mzg = greenIntensity; mzb = 0;
        mer = redIntensity; meg = greenIntensity; meb = 0;
        hzr = redIntensity; hzg = greenIntensity; hzb = 0;
        her = redIntensity; heg = greenIntensity; heb = 0;        
         _redIntensity = redIntensity;
         _greenIntensity = greenIntensity;        
        if(_greenIntensity >= max_zero_bright){_greenIntensity = max_zero_bright;}
         if(_redIntensity >= max_zero_bright){_redIntensity = max_zero_bright;}     
        szrz =_redIntensity; szgz = _greenIntensity; szbz = 0;
        serz = _redIntensity; segz = _greenIntensity; sebz = 0;
        mzrz = _redIntensity; mzgz = _greenIntensity; mzbz = 0;
        merz = _redIntensity; megz = _greenIntensity; mebz = 0;
        hzrz = _redIntensity; hzgz = _greenIntensity; hzbz = 0;
        herz = _redIntensity; hegz = _greenIntensity; hebz = 0;      
        if(stop_effect == 1){stop_effect = 0;break;}
         getDateDs1307();
          button();
            webui();
        delay(main_delay);
  } 
  for (blueIntensity = 0; blueIntensity <= max_bright; blueIntensity+=step_width) {
        greenIntensity = 255-blueIntensity;        
        szr =0; szg = greenIntensity; szb = blueIntensity;
        ser = 0; seg = greenIntensity; seb = blueIntensity;
        mzr = 0; mzg = greenIntensity; mzb = blueIntensity;
        mer = 0; meg = greenIntensity; meb = blueIntensity;
        hzr = 0; hzg = greenIntensity; hzb = blueIntensity;
        her = 0; heg = greenIntensity; heb = blueIntensity;       
       _greenIntensity =greenIntensity;
      _blueIntensity =blueIntensity;   
        if(_greenIntensity >= max_zero_bright){_greenIntensity = max_zero_bright;}
         if(_blueIntensity >= max_zero_bright){_blueIntensity = max_zero_bright;}        
         szrz =0; szgz = _greenIntensity; szbz = _blueIntensity;
        serz = 0; segz = _greenIntensity; sebz = _blueIntensity;
        mzrz = 0; mzgz = _greenIntensity; mzbz = _blueIntensity;
        merz = 0; megz = _greenIntensity; mebz = _blueIntensity;
        hzrz = 0; hzgz = _greenIntensity; hzbz = _blueIntensity;
        herz = 0; hegz = _greenIntensity; hebz = _blueIntensity;       
        if(stop_effect == 1){stop_effect = 0;break;}
         getDateDs1307();
          button();
            webui();
        delay(main_delay);
  }    
  for (redIntensity = 0; redIntensity <= max_bright; redIntensity+=step_width) {
        blueIntensity = 255-redIntensity;    
        szr =redIntensity; szg = 0; szb = blueIntensity;
        ser = redIntensity; seg = 0; seb = blueIntensity;
        mzr = redIntensity; mzg = 0; mzb = blueIntensity;
        mer = redIntensity; meg = 0; meb = blueIntensity;
        hzr = redIntensity; hzg = 0; hzb = blueIntensity;
        her = redIntensity; heg = 0; heb = blueIntensity;      
        _redIntensity = redIntensity;
          _blueIntensity =blueIntensity;         
         if(_redIntensity >= max_zero_bright){_redIntensity = max_zero_bright;}
         if(_blueIntensity >= max_zero_bright){_blueIntensity = max_zero_bright;}      
         szrz =_redIntensity; szgz = 0; szbz = _blueIntensity;
        serz = _redIntensity; segz = 0; sebz = _blueIntensity;
        mzrz = _redIntensity; mzgz = 0; mzbz = _blueIntensity;
        merz = _redIntensity; megz = 0; mebz = _blueIntensity;
        hzrz = _redIntensity; hzgz = 0; hzbz = _blueIntensity;
        herz = _redIntensity; hegz = 0; hebz = _blueIntensity;      
        if(stop_effect == 1){stop_effect = 0;break;}
          getDateDs1307();
          button();
            webui();
        delay(main_delay);
  }
}else{
ledmode = 4;
}
}




void button(){
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
   if(buff_h >= 23){
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
   if(buff_s >= 23){
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
}else if( digitalRead(sw1) == HIGH && digitalRead(sw2) == HIGH && digitalRead(sw3) == LOW && prev_state_button == 0){
prev_state_button = 1;
Serial.println("START ZEITZONEN EDIT");
int buff_zz = EEPROM.read(3);
int prev_state_zz = 0;
while (digitalRead(sw1) == HIGH &&  digitalRead(sw2) == HIGH){ 
 if( digitalRead(sw3) == HIGH){
   prev_state_zz = 1;  
   if(buff_zz >= 23){
   buff_zz = 0;
   }else{
   buff_zz++;
   }  
   zeitverschiebung = buff_zz;
Serial.print("ZZ");   Serial.println(zeitverschiebung);
 }else{
 prev_state_zz = 0;
 }
display_time(0,buff_zz,0);
delay(200);
}//END WHILE
EEPROM.write(3,buff_zz); //NT
zeitverschiebung = buff_zz;
get_ntp_time(); 
 
   delay(1000);
}else if( digitalRead(sw1) == LOW && digitalRead(sw2) == LOW && digitalRead(sw3) == HIGH && prev_state_button == 0){
prev_state_button = 1;
Serial.println("START NTP REQUEST");
  get_ntp_time(); 
  
   delay(1000);   
}else if( digitalRead(sw1) == LOW && digitalRead(sw2) == HIGH && digitalRead(sw3) == LOW && prev_state_button == 0){ 
  Serial.print("CHANGE MODE : "); Serial.println(ledmode); 
   prev_state_button = 1;
   if(ledmode == maxmode){
   ledmode = 0;
   }else{
   ledmode++;
   }
   stop_effect = 1;
}else{
if(digitalRead(sw1) == LOW&& digitalRead(sw2) == LOW && digitalRead(sw3) == LOW ){ 
  prev_state_button = 0;
} 
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
  display_time(0,0,0);
  Serial.println("START NTP REQUEST"); 
  if(use_ethernet == 1){
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
    
    Serial.print("NTP-TIME : "); Serial.print(ntph); Serial.print(" : "); Serial.print(ntpm);Serial.print(" : "); Serial.println(ntps);
    delay(500);
    Serial.print("START WRITE TO RTC // TIMEZONE : +");Serial.println(zeitverschiebung);
    second = ntps;
   minute = ntpm;
   hour = ntph;
   //hier die zeitverschiebung beachten
   hour  = hour+zeitverschiebung;
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
   I2C_WRITE(decToBcd(hour));      // If you want 12 hour am/pm you need to set                                  // bit 6 (also need to change readDateDs1307)
   I2C_WRITE(decToBcd(dayOfWeek));
   I2C_WRITE(decToBcd(dayOfMonth));
   I2C_WRITE(decToBcd(month));
   I2C_WRITE(decToBcd(year));
   Wire.endTransmission(); 
  }
  }
}





 
      
      
      
