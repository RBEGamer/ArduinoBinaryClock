/*
TASTENBELEGUNG : 
Taster 3 kurz drücken : get server time (internet muss vorhanden sein)
Taster 2 kurz drücken: änder LED Mode
Taster 1+2 gedrückt halten: mit Taster 2 die zeitzone immer +1h ändern. dann Taster 1+2 loslassen und taster 3 kurz drücken um diese zu übernhemen und get server time (internet muss vorhanden sein)
Taster 1 gedrückt halten : nighmode ändern : Taster 2 kurz drücken : nighmode an/aus
                                              Taster 1 : nighmode anfangsstunde ändern immer +1h
                                             Taster 3 : nighmode endstunde ändern immer +1h





*/






/* you can change the authentication realm by defining
 * WEBDUINO_AUTH_REALM before including WebServer.h */
#define WEBDUINO_AUTH_REALM "Weduino Authentication Example"


#include <FastSPI_LED.h>
#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "Wire.h"
#include <EEPROM.h>
#include <String>
#include "WebServer.h"
#include <aJSON.h>

 //-------- USER SETTINGS-----------------
 const int sw1 = 6; //SCHALTER 1 PIN
const int sw2 = 5; //SCHALTER 2 PIN
const int sw3 = 4; //SCHALTER 3 PIN
 const int beeper_pin = 2;  //BEEPR PIN
#define PIN 7 //PIN DER LEDS
 IPAddress timeServer(132, 163, 4, 103); // HIER DIE NTC SERVER ADRESSE EINTRAGEN
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //MACADRESSE EINTRAGEN 
int enable_dhcp = 1;
IPAddress ip(192,168,178, 199);
IPAddress gateway(192,168,178, 1);
IPAddress subnet(255, 255, 255, 0);
const int webui_port = 80;


char *webui_credentials = "YWRtaW46MTU5MA==";//USER INTERFACE LOGINDATEN admin:1590 //http://www.base64encode.org/
 
 
 //----END USER SETTINGS-----------------
 

const int step_width = 1; //SCHNELLIGKEIT DES FADES 1
 int max_zero_bright = 5; //HELLIGKEIT LED AUS
const int main_delay = 100; //SYSTEM DELAY NICHT SCHNELLER ALS 100
 int max_bright = 70; //HELLIGKEIT LED AN

//FÜR SERVER << "";
 template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }
#define PREFIX ""
WebServer webserver(PREFIX, webui_port);
#define NAMELEN 64
#define VALUELEN 64
const int post_buffer_size = 64;


 const int use_webui = 1;
 const int use_ethernet = 1; //SOLL ETHERNET BENUTZT WERDEN
 const int wait_for_DHCP = 0;




const int software_version = 2501;
int alarm_en = 0;
int alarm_h = 0;
int alarm_m = 0;
int alarm_process = 0;
int alarm_state = 0;
int alarm_button_enable = 0;


int last_ntp_h = 0;
int last_ntp_m = 0;
int last_gps_h = 0;
int last_gps_m = 0;
float gps_npos;
float gps_opos;

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
 
 P(head) = "<html><head>"
"<meta http-equiv='Content-Type' content='text/html; charset=windows-1252'>"
"<title>EPIC NERD CLOCK ADMIN PAGE</title>"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'/>"
"<meta name='apple-mobile-web-app-capable' content='yes'/>"
"<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'/>"
"<link rel='stylesheet' href='http://clock.rbnetworks.de/chromoselector.css'/>"
"<style type='text/css'>a:link{color:#FFF;}a:visited{color:#FFF;}a:hover{color:#FFCA00;}a:active{color:#FFAA00;}"


        "button {"
	"font-family: Arial, Helvetica, sans-serif;"
	"font-size: 14px;"
	"color: #ffffff;"
	"padding: 10px 20px;"
	"background: -moz-linear-gradient("
	"top,"
		"#001527 0%,"
		"#3b3b3b 50%,"
		"#242424 50%,"
		"#000000);"
	"background: -webkit-gradient("
	"	linear, left top, left bottom,"
	"	from(#001527),"
	"	color-stop(0.50, #3b3b3b),"
	"	color-stop(0.50, #242424),"
	"	to(#000000));"
	"-moz-border-radius: 10px;"
	"-webkit-border-radius: 10px;"
	"border-radius: 10px;"
	"border: 1px solid #000000;"
	"-moz-box-shadow:"
       	"	0px 1px 3px rgba(000,000,000,0.5),"
	"	inset 0px 0px 1px rgba(255,255,255,0.6);"
	"-webkit-box-shadow:"
	"	0px 1px 3px rgba(000,000,000,0.5),"
	"	inset 0px 0px 1px rgba(255,255,255,0.6);"
	"box-shadow:"
	"	0px 1px 3px rgba(000,000,000,0.5),"
	"	inset 0px 0px 1px rgba(255,255,255,0.6);"
	"text-shadow:"
	"	0px -1px 0px rgba(000,000,000,1),"
	"	0px 1px 0px rgba(255,255,255,0.2);"
"}"

"</style>"
"<script src='http://clock.rbnetworks.de/jquery-1.10.2.min.js' type='text/javascript'></script>"
"<script src='http://clock.rbnetworks.de/chromoselector.min.js' type='text/javascript'></script>"
"<script src='http://clock.rbnetworks.de/colorbox.js' type='text/javascript'></script>"
"</head>";
   P(page_start) = "<body topmargin='0' leftmargin='0' rightmargin='0' bottommargin='0' marginheight='0' marginwidth='0' bgcolor='#FFFFFF'><center>"
"<table border='0' width='100%' cellspacing='0' cellpadding='0' background='http://clock.rbnetworks.de/img/topbkg.gif'><tr>"
"<td width='50%'><img border='0' src='http://clock.rbnetworks.de/img/topleft.gif' width='205' height='61'></td>"
"<td width='50%'><p align='right'><img border='0' src='http://clock.rbnetworks.de/img/topright.gif' width='205' height='61'></td></tr>"
"</table>"
"<table border='0' width='100%' cellspacing='0' cellpadding='0' background='http://clock.rbnetworks.de/img/linebkg.gif'><tr>"
"<td><a href='/'>STATUS</a></td>"
"<td><a href='/color.html'>COLOR SETTINGS</a></td>"
"<td><a href='/alarm.html'>ALARM SETTINGS</a></td>"
"<td><a href='/night.html'>NIGHTMODE SETTINGS</a></td>"
//"<td><a href='/general.html'>TWITTER SETTINGS</a></td>"
//"<td><a href='/network.html'>NETWORK SETTINGS</a></td>"
"<td><a href='/general.html'>GENERAL SETTINGS</a></td>"
"</tr></table>"
"<p style='margin-left: 20'><font face='Arial' size='2'>&nbsp;</font></p>";
P(page_end) = "<table border='0' width='100%' background='http://clock.rbnetworks.de/img/botbkg.gif' cellspacing='0' cellpadding='0'>"
   "<tr>"
"<td width='100%'><img border='0' src='http://clock.rbnetworks.de/img/botbkg.gif' width='38' height='14'></td>"
"</tr>"
"</table>"
   "</center></body></html>";
   
   
  // <button type="button" name="" value="" class='css3button'>submit</button>
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
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
 
 
 
 
 
 
 








void setup() {
      Serial.println("");



  
   pinMode(sw1, INPUT);
    pinMode(sw2, INPUT);
     pinMode(sw3, INPUT);  
     
     pinMode(beeper_pin, OUTPUT);
     stop_effect = 0;  

     
     
load_settings();
 
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
    
   // start Ethernet and UDP
   
   if(enable_dhcp == 1){
     Serial.println("DHCP ENAVBLED : Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
 if(wait_for_DHCP == 1){
  for(;;)
      ;
 }else{
    Serial.println("Failed to configure Ethernet using DHCP : TRY TO USE ALTERNATIVE IP CONFIGURATION");
  Ethernet.begin(mac, ip, gateway, subnet);
 }
   
   
   
  }
  
  
   }else{
     Serial.println("DHCP DISABLED : TRY TO USE ALTERNATIVE IP CONFIGURATION");
     Ethernet.begin(mac, ip, gateway, subnet);
   
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
start_webui();
  }

 
  }
  
  
  
}
 




 
 void webui(){
if(use_webui == 1){
  char buff[post_buffer_size];
  int len = post_buffer_size;
  webserver.processConnection(buff, &len);
 }
}


void switch_ledmode(int mode){
  Serial.print("switch LED-Mode from  ");Serial.print(ledmode);Serial.print(" to  ");Serial.print(mode);
ledmode = mode;
stop_effect = 1;

}





void restore_settings(){
nth = 22;
nthe = 6;
nthen = 1;
zeitverschiebung = 2; //GTM+2h
enable_dhcp = 1;
alarm_en = 0;
alarm_h = 6;
alarm_m = 0;
nthr = 10;nthg = 0; nthb = 0;
nthzr = 0;nthzg = 0; nthzb = 5; //timeServer(132, 163, 4, 103)
timeServer[0] = 132;
timeServer[1] = 163;
timeServer[2] = 4;
timeServer[3] = 103;
ip[0] = 192;
ip[1] = 168;
ip[2] = 178;
ip[3] = 58;
subnet[0] = 255;
subnet[1] = 255;
subnet[2] = 255;
subnet[3] = 0;
gateway[0] = 192;
gateway[1] = 168;
gateway[2] = 178;
gateway[3] = 1;
alarm_button_enable = 1;
ledmode = 2;
last_gps_h = 0;
last_gps_m = 0;
last_gps_h = 0;
last_gps_m = 0;
save_settings();
}

void save_settings(){
 Serial.println("SAFE ALL SETTINGS TO EEPROM"); 
EEPROM.write(0,nth); //NT
EEPROM.write(1,nthe); //NT
EEPROM.write(2,nthen); //NT
EEPROM.write(3,zeitverschiebung); //NT
EEPROM.write(4,max_zero_bright); //NT
EEPROM.write(5,max_bright); //NT
EEPROM.write(8,enable_dhcp); //NT
EEPROM.write(9,alarm_en); //NT
EEPROM.write(10,alarm_h); //NT
EEPROM.write(11,alarm_m); //NT
EEPROM.write(12,nthr); //NT
EEPROM.write(13,nthg); //NT
EEPROM.write(14,nthb); //NT
EEPROM.write(15,nthzr); //NT
EEPROM.write(16,nthzg); //NT
EEPROM.write(17,nthzb); //NT
EEPROM.write(18,timeServer[0]); //NT
EEPROM.write(19,timeServer[1]); //NT
EEPROM.write(20,timeServer[2]); //NT
EEPROM.write(21,timeServer[3]); //NT
EEPROM.write(22,ip[0]); //NT
EEPROM.write(23,ip[1]); //NT
EEPROM.write(24,ip[2]); //NT
EEPROM.write(25,ip[3]); //NT
EEPROM.write(26,subnet[0]); //NT
EEPROM.write(27,subnet[1]); //NT
EEPROM.write(28,subnet[2]); //NT
EEPROM.write(29,subnet[3]); //NT
EEPROM.write(30,gateway[0]); //NT
EEPROM.write(31,gateway[1]); //NT
EEPROM.write(32,gateway[2]); //NT
EEPROM.write(33,gateway[3]); //NT
EEPROM.write(34,alarm_button_enable); //NT


EEPROM.write(35,ser); //NT
EEPROM.write(36,seg); //NT
EEPROM.write(37,seb); //NT
EEPROM.write(38,szr); //NT
EEPROM.write(39,szg); //NT
EEPROM.write(40,szb); //NT

EEPROM.write(41,serz); //NT
EEPROM.write(42,segz); //NT
EEPROM.write(43,sebz); //NT
EEPROM.write(44,szrz); //NT
EEPROM.write(45,szgz); //NT
EEPROM.write(46,szbz); //NT


EEPROM.write(47,mer); //NT
EEPROM.write(48,meg); //NT
EEPROM.write(49,meb); //NT
EEPROM.write(50,mzr); //NT
EEPROM.write(51,mzg); //NT
EEPROM.write(52,mzb); //NT

EEPROM.write(53,merz); //NT
EEPROM.write(54,megz); //NT
EEPROM.write(55,mebz); //NT
EEPROM.write(56,mzrz); //NT
EEPROM.write(57,mzgz); //NT
EEPROM.write(58,mzbz); //NT



EEPROM.write(59,her); //NT
EEPROM.write(60,heg); //NT
EEPROM.write(61,heb); //NT
EEPROM.write(62,hzr); //NT
EEPROM.write(63,hzg); //NT
EEPROM.write(64,hzb); //NT

EEPROM.write(65,herz); //NT
EEPROM.write(66,hegz); //NT
EEPROM.write(67,hebz); //NT
EEPROM.write(68,hzrz); //NT
EEPROM.write(69,hzgz); //NT
EEPROM.write(70,hzbz); //NT

EEPROM.write(71,ledmode); //NT

EEPROM.write(72,last_ntp_h); //NT
EEPROM.write(73,last_ntp_m); //NT
EEPROM.write(74,last_gps_h); //NT
EEPROM.write(75,last_gps_m); //NT

EEPROM.write(76,gps_npos); //NT
EEPROM.write(77,gps_opos); //NT


}




void load_settings(){
   Serial.println("READ ALL SETTINGS TO EEPROM"); 
     nth = EEPROM.read(0);
     nthe = EEPROM.read(1);
     nthen = EEPROM.read(2);
     zeitverschiebung = EEPROM.read(3);
     max_zero_bright =  EEPROM.read(4);
     max_bright =  EEPROM.read(5);
     enable_dhcp = EEPROM.read(8);  
   alarm_en =  EEPROM.read(9);
 alarm_h =  EEPROM.read(10);
 alarm_m =  EEPROM.read(11);
  nthr =  EEPROM.read(12);
    nthg =  EEPROM.read(13);
      nthb =  EEPROM.read(14);
        nthzr =  EEPROM.read(15);
    nthzg =  EEPROM.read(16);
      nthzb =  EEPROM.read(17);
timeServer[0] = EEPROM.read(18);
timeServer[1] = EEPROM.read(19);
timeServer[2] = EEPROM.read(20);
timeServer[3] = EEPROM.read(21);
ip[0] = EEPROM.read(22);
ip[1] = EEPROM.read(23);
ip[2] = EEPROM.read(24);
ip[3] = EEPROM.read(25);
subnet[0] = EEPROM.read(26);
subnet[1] = EEPROM.read(27);
subnet[2] = EEPROM.read(28);
subnet[3] = EEPROM.read(29);

gateway[0] = EEPROM.read(30);
gateway[1] = EEPROM.read(31);
gateway[2] = EEPROM.read(32);
gateway[3] = EEPROM.read(33);
alarm_button_enable = EEPROM.read(34);



ser = EEPROM.read(35);
seg = EEPROM.read(36);
seb = EEPROM.read(37);
szr = EEPROM.read(38);
szg = EEPROM.read(39);
szb = EEPROM.read(40);

serz = EEPROM.read(41);
segz = EEPROM.read(42);
sebz = EEPROM.read(43);
szrz = EEPROM.read(44);
szgz = EEPROM.read(45);
szbz = EEPROM.read(46);

mer = EEPROM.read(47);
meg = EEPROM.read(48);
meb = EEPROM.read(49);
mzr = EEPROM.read(50);
mzg = EEPROM.read(51);
mzb = EEPROM.read(52);

merz = EEPROM.read(53);
megz = EEPROM.read(54);
mebz = EEPROM.read(55);
mzrz = EEPROM.read(56);
mzgz = EEPROM.read(57);
mzbz = EEPROM.read(58);


her = EEPROM.read(59);
heg = EEPROM.read(60);
heb = EEPROM.read(61);
hzr = EEPROM.read(62);
hzg = EEPROM.read(63);
hzb = EEPROM.read(64);

herz = EEPROM.read(65);
hegz = EEPROM.read(66);
hebz = EEPROM.read(67);
hzrz = EEPROM.read(68);
hzgz = EEPROM.read(69);
hzbz = EEPROM.read(70);


ledmode = EEPROM.read(71);

last_ntp_h = EEPROM.read(72);
last_ntp_m = EEPROM.read(73);
last_gps_h = EEPROM.read(74);
last_gps_m = EEPROM.read(75);

gps_npos = EEPROM.read(76);
gps_opos = EEPROM.read(77);
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
main_prcess();
}else if(ledmode == 1){
 
main_prcess();
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
main_prcess();
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
   main_prcess();
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
 main_prcess();
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
  main_prcess();
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
    main_prcess();
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
      main_prcess();
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
        main_prcess();
  }
}else{
ledmode = 1;
main_prcess();
}


main_prcess();
}




void button(){
  if(alarm_button_enable== 1){
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
Serial.println("START GPS REQUEST");
  get_gps_time(); 
  
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
   
   
   }else if( digitalRead(sw1) == HIGH && digitalRead(sw2) == HIGH && digitalRead(sw3) == HIGH && prev_state_button == 0){ 
  //Serial.print("CHANGE MODE : "); Serial.println(ledmode); 
   prev_state_button = 1;
   stop_alarm();
   
}else{
if(digitalRead(sw1) == LOW&& digitalRead(sw2) == LOW && digitalRead(sw3) == LOW ){ 
  prev_state_button = 0;
} 
}
//END IF HIGH
}
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
   
      last_ntp_h = hour;
      last_ntp_m = minute;
   
   
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



void get_gps_time(){
  display_time(0,0,0);
  Serial.println("START GPS REQUEST"); 


    Serial.print("NTP-TIME : "); Serial.print(ntph); Serial.print(" : "); Serial.print(ntpm);Serial.print(" : "); Serial.println(ntps);
    delay(500);
    Serial.print("START WRITE TO RTC // TIMEZONE : +");Serial.println(zeitverschiebung);
    gps_npos= 51.336751;
    gps_opos = 6.616647;
//    second = ntps;
//   minute = ntpm;
//   hour = ntph;
   //hier die zeitverschiebung beachten
   hour  = hour+zeitverschiebung;
   if(hour == 24){
   hour = 0;
   }if(hour > 24){   
   hour = hour -24;
   }   
   
   last_gps_h = hour;
   last_gps_m = minute;
   
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
  






      void main_prcess(){


      
        getDateDs1307();
          button();
            webui();
            soundAlarm();
        delay(main_delay);
      }
      
      

void stop_alarm(){
    if(alarm_state == 1 && alarm_en ==1){
    alarm_state = 0;
  while(minute == alarm_m){ //SOLANGE warten bis die zeit vorbei ist
  webui();
  Serial.println("ALARM DELAY WHILE"); 
  getDateDs1307();
  
  
  }
 
 alarm_process = 0; 
  alarm_state = 0; 
  delay(2000);
  
  
    }
}


void soundAlarm() {
  
  if(hour == alarm_h && minute == alarm_m && alarm_en == 1 && alarm_process == 0 ){
    alarm_process = 1;
    alarm_state = 1;
    
    
  }else{
 
  }
  

  
  if(alarm_state == 1 && alarm_en ==1){
 float alarmFrequency=1400; // The value for the alarm tone in Hz
 float period = (1.0 / alarmFrequency) * 1000000;
 long beepDuration=250000; // the time in microseconds (0.25 seconds)
 long elapsedTime = 0;
 while (elapsedTime < beepDuration) {
 digitalWrite(beeper_pin,HIGH);
 delayMicroseconds(period / 2);
 digitalWrite(beeper_pin, LOW);
 delayMicroseconds(period / 2);
 elapsedTime += (period);
 }
 digitalWrite(beeper_pin, LOW); 
 delayMicroseconds(beepDuration);
 }
  
}

/*
void networkCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete){
     URLPARAM_RESULT rc;
      char name[NAMELEN];char value[VALUELEN];
if (server.checkCredentials(webui_credentials)){ 
  server.httpSuccess();
  if (type != WebServer::HEAD)
  { 
     server.printP(head);
     server.printP(page_start); 
 //_-----------------------------    
 server<<"<div ><h2>NETWORK SETTINGS</h2></div><hr>";
server<<"  <form id='network' name='network' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"  <tr>";
server<<"    <td scope='col'><label for='ntpip'>ALTERNATIVE IP</label></td>";
server<<"    <td scope='col' colspan='3'><input id='ntpip' name='ntpip' type='text' value='192,168,178,1' placeholder='[xxx,xxx,xxx,xxx]' /></td>";
server<<"    <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<" </tr> ";
server<<"    </table>";
server<<"    </form>";
server<<"<form id='network' name='network' action=''  method='post'>";
server<<"<table width='50%' border='0'>";
server<<"  <tr> ";
server<<"   <td><label for='nth_en'>DHCP STATE *</label> </td>";
server<<"    <td scope='col'><input type='radio' name='dhcp' id='dhcp' value='0' >DISABLE</td>  ";
server<<"    <td scope='col'><input type='radio' name='dhcp' id='dhcp'  value='1' checked>ENABLE </td>  ";
server<<"     <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<"  </tr>   ";
server<<"   </table>";
server<<"   </form>  ";
server<<"   <form id='network' name='network' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"  <tr>";
server<<"    <td scope='col'><label for='ip'>ALTERNATIVE IP *</label></td>";
server<<"    <td scope='col' colspan='3'><input id='ip' name='ip' type='text' value='192,168,178,1' placeholder='[xxx,xxx,xxx,xxx]' /></td>";
server<<"    <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<" </tr> ";
server<<"    </table>";
server<<"    </form>   ";
server<<"      <form id='network' name='network' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"  <tr>";
server<<"    <td scope='col'><label for='subnet'>ALTERNATIVE SUBNET *</label></td>";
server<<"    <td scope='col' colspan='3'><input id='subnet' name='subnet' type='text' value='255,255,255,0' placeholder='[xxx,xxx,xxx,xxx]' /></td>";
server<<"    <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<" </tr> ";
server<<"    </table>";
server<<"    </form> ";
server<<"          <form id='network' name='network' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"  <tr>";
server<<"    <td scope='col'><label for='gateway'>ALTERNATIVE GATEWAY *</label></td>";
server<<"    <td scope='col' colspan='3'><input id='gateway' name='gateway' type='text' value='192,168,178,1' placeholder='[xxx,xxx,xxx,xxx]' /></td>";
server<<"    <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<" </tr> ";
server<<"    </table>";
server<<"    </form>  ";
server<<"  * REQUIRE CLOCK REBOOT  ";  
//_----------------------------------
      server.printP(page_end);      
  }
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
   post_process(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 post_process(name,value);
    }
  } 
}
*/
P(page_alarm) ="<form id='stopalarm' name='stopalarm' action=''  method='post'>"
"<input type='hidden' id='stopalarm' name='stopalarm' value='1'>"
"<input type='submit' value='!!! STOP ALARM !!!' />"
"</form>";

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete){
     URLPARAM_RESULT rc;
      char name[NAMELEN];char value[VALUELEN];
      
      P(page_update_button)= "   <form id='gettime' name='gettime' action=''  method='post'>"
"<input type='hidden' id='getntp' name='getntp' value='1'>"
"<input type='submit' value='UPDATE TIME VIA NTP-SERVER' />"
"</form>"
"   <form id='gettime' name='gettime' action=''  method='post'>"
"<input type='hidden' id='getgps' name='getgps' value='1'>"
"<input type='submit' value='UPDATE TIME VIA GPS' />"
"</form>";
      
      
if (server.checkCredentials(webui_credentials)){ 
  server.httpSuccess();
  if (type != WebServer::HEAD)
  { 

 
 
 if(alarm_en == 1 && alarm_state == 1){
  server.printP(head);
     server.printP(page_start); 
      server.printP(page_alarm);
     server.printP(page_end); 

 
 }else{
      server.printP(head);
     server.printP(page_start); 
 //_-----------------------------    
 server<<"<div><h2> CURRENT TIME: ";if(hour < 10){server<< "0"<< hour;}else{server<<hour;} server<<":";if(minute < 10){server<< "0"<< minute;}else{server<<minute;}server<<":";if(second < 10){server<< "0"<< second;}else{server<<second;} server<<"</h2></div>";


server.printP(page_update_button);


server <<"<hr><div><h3>SOFTWARE-VERSION : ";server<<software_version ;server <<"</h3></div>";
server <<"<hr><div><h3>WebUI : ";server<<ip[0]; server<<"."; server<<ip[1]; server<<".";server<<ip[2]; server<<".";server<<ip[3]; server<<":"; server<< webui_port; server <<"</h3></div>";
//server <<"<hr><div><h3>GPS SATELITES : ";server<<"8";server <<"</h3></div>";
server <<"<hr><div><h3>LAST TIME UPDATE [NTP] : ";if(last_ntp_h < 10){server<<"0";}server<< last_ntp_h; server<<":";if(last_ntp_m < 10){server<<"0";} server<<last_ntp_m ;server <<"</h3></div>";
server <<"<hr><div><h3>LAST TIME/POSITION UPDATE  [GPS] : ";if(last_gps_h < 10){server<<"0";}server<< last_gps_h; server<<":";if(last_gps_m < 10){server<<"0";} server<<last_gps_m ;server <<"</h3></div>";



server<<"<hr><div><h3>CLOCK POSITION : N";server<<gps_npos; server<<"  O";server<<gps_opos;server<<"</h3></div>";


//_----------------------------------
      server.printP(page_end);  
 }
 
    
  }
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
   post_process(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 post_process(name,value);
    }
  } 
}



void colorCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete){
     URLPARAM_RESULT rc;
      char name[NAMELEN];char value[VALUELEN];
if (server.checkCredentials(webui_credentials)){ 
  server.httpSuccess();
  if (type != WebServer::HEAD)
  { 
     if(alarm_en == 1 && alarm_state == 1){
  server.printP(head);
     server.printP(page_start); 
      server.printP(page_alarm);
     server.printP(page_end); 

 
 }else{
      server.printP(head);
     server.printP(page_start); 
 //_-----------------------------    
 server<<"<div ><h2>COLOR SETTINGS</h2></div>";
server<<"<hr>";

server<<" <form id='colorsettings' name='colorsettings' action='' method='post'>";
server<<"<table width='50%' border='0'>";
server<<"  <tr>";
server<<"    <td><label for='ledmode'>LED EFFECT</label> </td>";
server<<"    <td scope='col'><input type='radio' name='mode' id='mode' value='0' ";if(ledmode == 0){server <<"checked";}server<<">OFF</td>  ";
server<<"    <td scope='col'><input type='radio' name='mode' id='mode'  value='1' ";if(ledmode == 1){server <<"checked";}server<<">CUSTOM </td>";
server<<"     <td scope='col'><input type='radio' name='mode' id='mode'  value='2' ";if(ledmode == 2){server <<"checked";}server<<">RAINBOW</td>";
server<<" <td><input type='submit' value='SAVE' /></td> ";
server<<"  </tr>  ";
server<<"</table>";
server<<"</form>";




server<<" <form id='colorsettings' name='colorsettings' action='' method='post'>";
server<<"<table width='50%' border='0'>";
server<<"   <tr>";
server<<"    <td scope='col'> <label for='custom_on_h'>CUSTOM MODE <br> LED ON</label></td>";
server<<"    <td>";
server<<"    HOUR : <br>";
server<<"    MINUTE : <br>";
server<<"    SECOND : ";
server<<"    </td> ";
server<<"    <td scope='col' colspan='3'>";
server<<"      <input id='custom_on_h' name='conh' type='text' value=' ";server<< hzb;server<<",";server<< hzr;server<<",";server<< hzg;server<<"' placeholder='[R,G,B]' /><br>";
server<<"     <input id='custom_on_m' name='conm' type='text' value=' ";server<< mzb;server<<",";server<< mzr;server<<",";server<< mzg;server<<"' placeholder='[R,G,B]' /><br>";
server<<"      <input id='custom_on_s' name='cons' type='text' value=' ";server<< szb;server<<",";server<< szr;server<<",";server<< szg;server<<"' placeholder='[R,G,B]' />";
server<<"     </td>  ";
server<<" <td><input type='submit' value='SAVE' /></td> ";
server<<"  </tr>";
server<<"</table>";
server<<"</form>";



server<<" <form id='colorsettings' name='colorsettings' action='' method='post'>";
server<<"<table width='50%' border='0'>";
server<<"   <tr>";
server<<"    <td scope='col'> <label for='custom_off_h'>CUSTOM MODE <br> LED OFF</label></td>";
server<<"    <td>";
server<<"    HOUR : <br>";
server<<"    MINUTE : <br>";
server<<"    SECOND : ";
server<<"    </td>   ";
server<<"    <td scope='col' colspan='3'>";
server<<"      <input id='custom_off_h' name='coh' type='text' value=' ";server<< hzbz;server<<",";server<< hzrz;server<<",";server<< hzgz;server<<"' placeholder='[R,G,B]' /><br>";
server<<"     <input id='custom_off_m' name='com' type='text' value=' ";server<< mzbz;server<<",";server<< mzrz;server<<",";server<< mzgz;server<<"' placeholder='[R,G,B]' /><br>";
server<<"      <input id='custom_off_s' name='cos' type='text' value=' ";server<< szbz;server<<",";server<< szrz;server<<",";server<< szgz;server<<"' placeholder='[R,G,B]' />";
server<<"     </td>  ";
server<<" <td><input type='submit' value='SAVE' /></td> ";
server<<"  </tr>  ";

server<<"</table>";
server<<"</form>" ; 
//_----------------------------------
      server.printP(page_end);      
  }
  }
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
     post_process(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
      
            post_process(name,value);
    }
  } 
}






void alarmCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete){
     URLPARAM_RESULT rc;
      char name[NAMELEN];char value[VALUELEN];
if (server.checkCredentials(webui_credentials)){ 
  server.httpSuccess();
  if (type != WebServer::HEAD)
  { 
if(alarm_en == 1 && alarm_state == 1){
  server.printP(head);
     server.printP(page_start); 
      server.printP(page_alarm);
     server.printP(page_end); 

 
 }else{
      server.printP(head);
     server.printP(page_start); 
 //_-----------------------------    
server<<"<div ><h2>ALARM SETTINGS</h2></div>";
server<<"<hr>";
server<<" <form id='alarmsettings' name='alarmsettings' action=''  method='post'>";
server<<"<table width='50%' border='0'>";
server<<"  <tr> ";
server<<"   <td><label for='alarm_en'>ALARM STATUS</label> </td>";
server<<"    <td scope='col'><input type='radio' name='alarm_en' id='alarm_en' value='0' ";if(alarm_en == 0){server <<"checked";}server<<">DISABLE</td>  ";
server<<"    <td scope='col'><input type='radio' name='alarm_en' id='alarm_en'  value='1' ";if(alarm_en == 1){server <<"checked";}server<<">ENABLE </td>";
server<<"     <td colspan='1'><input type='submit' value='SAVE' /></td>";
server<<"  </tr>  ";
server<<"   </table>";
server<<"   <form id='alarmsettings' name='alarmsettings' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"   <tr>";
server<<"    <td scope='col'> <label for='alarm_h'>ALARM TIME</label></td>";
server<<"    <td>";
server<<"    HOUR : <br>";
server<<"    MINUTE : <br>";
server<<"    </td> ";
server<<"    <td scope='col' colspan='2>";
server<<"     <input name='a' type='number' id='a' max='23' min='0' step='1' value='99' size='5'><br>";
server<<"     <input name='alarm_h' type='number' id='alarm_h' max='23' min='0' step='1' value='"; server<<alarm_h;server <<"' size='5'><br>";

server<<"     <input name='alarm_m' type='number' id='alarm_m' max='59' min='0' step='1' value='"; server<<alarm_m;server <<"' size='5'><br>";
server<<"     </td> ";
server<<"     <td><input type='submit' value='SAVE' /></td> ";
server<<"  </tr>";
server<<"  </table>";    
//_----------------------------------
      server.printP(page_end);      
  }
  }
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
     post_process(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
post_process(name,value);
    }
  } 
}



void generalCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete){
     URLPARAM_RESULT rc;
      char name[NAMELEN];char value[VALUELEN];
if (server.checkCredentials(webui_credentials)){ 
  server.httpSuccess();
  if (type != WebServer::HEAD)
  { 
 if(alarm_en == 1 && alarm_state == 1){
  server.printP(head);
     server.printP(page_start); 
      server.printP(page_alarm);
     server.printP(page_end); 

 
 }else{
      server.printP(head);
     server.printP(page_start); 
 //_-----------------------------    
 server<<"<div ><h2>GENERAL SETTINGS</h2></div><hr>";
server<<" <form id='general' name='general' action=''  method='post'>";
server<<"<table width='50%' border='0'>";
server<<"  <tr> ";
server<<"   <td><label for='btn_en'>BUTTONS</label> </td>";
server<<"    <td scope='col'><input type='radio' name='btn_en' id='btn_en' value='0' ";if(alarm_button_enable == 0){server <<"checked";}server<<">DISABLE</td>  ";
server<<"    <td scope='col'><input type='radio' name='btn_en' id='btn_en'  value='1' ";if(alarm_button_enable == 1){server <<"checked";}server<<">ENABLE </td>  ";
server<<"     <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<"  </tr>   ";
server<<"   </table>";
server<<"   <form id='general' name='general' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"   <tr>";
server<<"    <td scope='col'> <label for='zz'>TIMEZONE </label></td>   ";
server<<"    <td scope='col' colspan='3'>";
server<<"      <input name='zz' type='number' id='zz' max='23' min='0' step='1' value='"; server<<zeitverschiebung;server <<"' >";
server<<" </td> ";
server<<"     <td><input type='submit' value='SAVE' /></td> ";
server<<"  </tr>";
server<<"   </table> ";
server<<"    </table>";
server<<" </form>";
server<<"   <form id='general' name='general' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"   <tr>";
server<<"    <td scope='col'> <label for='zz'>SET DEFAULT SETTINGS </label></td>";
server<<"    <td><input type='hidden' id='restore' name='restore' value='1'></td>   ";
server<<"     <td><input type='submit' value='RESTORE' /></td> ";
server<<"  </tr>";
server<<"   </table> ";
server<<" </form>";  
//_----------------------------------
      server.printP(page_end);      
  }
  }
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
     post_process(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 post_process(name,value);
    }
  } 
}








void nightCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete){
     URLPARAM_RESULT rc;
      char name[NAMELEN];char value[VALUELEN];
if (server.checkCredentials(webui_credentials)){ 
  server.httpSuccess();
  if (type != WebServer::HEAD)
  { 
if(alarm_en == 1 && alarm_state == 1){
  server.printP(head);
     server.printP(page_start); 
      server.printP(page_alarm);
     server.printP(page_end); 

 
 }else{
      server.printP(head);
     server.printP(page_start); 
 //_-----------------------------    
server<<"<div ><h2>NIGHTMODE SETTINGS</h2></div><hr>";
server<<" <form id='nightmode' name='nightmode' action=''  method='post'>";
server<<"<table width='50%' border='0'>";
server<<"  <tr> ";
server<<"   <td><label for='nth_en'>NIGHTMODE STATE</label> </td>";
server<<"    <td scope='col'><input type='radio' name='nth_en' id='nth_en' value='0' ";if(nthen == 0){server <<"checked";}server<<">DISABLE</td>  ";
server<<"    <td scope='col'><input type='radio' name='nth_en' id='nth_en'  value='1' ";if(nthen == 1){server <<"checked";}server<<">ENABLE </td>  ";
server<<"     <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<"  </tr>   ";
server<<"   </table>";
server<<"   </form>";
server<<"   <form id='nightmode' name='nightmode' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"   <tr>";

server<<"    <td scope='col'> <label for='nthco'>NIGHMODE LED-OFF COLOR</label></td>";
server<<"    <td scope='col' colspan='3'><input id='nthco' name='nthco' type='text' value='"; server<<nthzr;server<<",";server<<nthzg;server<<",";server<<nthzb;server <<"' placeholder='[R,G,B]' /></td> ";
server<<"    <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<"  </tr>";
server<<"  </table>";
server<<"   </form>";
server<<"   <form id='nightmode' name='nightmode' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"  <tr>";
server<<"    <td scope='col'><label for='nthco'>NIGHMODE LED-ON COLOR</label></td>";
server<<"    <td scope='col' colspan='3'><input id='nthca' name='nthca' type='text' value='"; server<<nthr;server<<",";server<<nthg;server<<",";server<<nthb;server <<"' placeholder='[R,G,B]' /></td>";
server<<"    <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<" </tr> ";
server<<"    </table>";
server<<"    </form>  ";
server<<"   <form id='nightmode' name='nightmode' action=''  method='post'>";
server<<"   <table width='50%' border='0'>";
server<<"   <tr>";
server<<"    <td scope='col'> <label for='nth'>NIGHTMODE ACTIVE TIME</label></td>";
server<<"    <td>";
server<<"    HOUR : <br>";
server<<"    MINUTE :  ";
server<<"    </td> ";
server<<"    <td scope='col' colspan='1'>";
server<<"    <input name='nth' type='number' id='nth' max='23' min='0' step='1' value='"; server<<nth;server <<"' size='5'><br>";
server<<"     <input name='nthe' type='number' id='nthe' max='23' min='0' step='1' value='"; server<<nthe;server <<"' size='5'><br>";
server<<"     </td>  ";
server<<"      <td colspan='2'><input type='submit' value='SAVE' /></td>";
server<<"  </tr>";
server<<"   </table> ";
server<<"   </form>";
//_----------------------------------
      server.printP(page_end);      
  }
  }
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
post_process(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 post_process(name,value);
    }
  } 
}








void post_process(char schluessel[256], char parameter[256]){
 
   int value0 = getValue(parameter, ',', 0).toInt(); // R / ip1
   int value1 = getValue(parameter, ',', 1).toInt(); //G / ip2
   int value2 = getValue(parameter, ',', 2).toInt();// B /ip3
   int value3 = getValue(parameter, ',', 3).toInt();//  /ip4
Serial.println(value0);
 process_response(schluessel, parameter, value0,value1,value2,value3);
 }

     //hzr = getValue(parameter, ',', 1).toInt();
 void process_response(char schluessel[256], char parameter[256], int value0, int value1, int value2, int value3){
if (strcmp(schluessel, "ledmode") == 0){
  //switch_ledmode(strtoul(parameter, NULL, 10));
}if (strcmp(schluessel, "alarm_h") == 0){
alarm_h = strtoul(parameter, NULL, 10);
}if (strcmp(schluessel, "alarm_m") == 0){
alarm_m = strtoul(parameter, NULL, 10);
 }if (strcmp(schluessel, "alarm_en") == 0){
  alarm_en = strtoul(parameter, NULL, 10);
}if (strcmp(schluessel, "btn_en") == 0){
 alarm_button_enable = strtoul(parameter, NULL, 10);
 }if (strcmp(schluessel, "restore") == 0){
restore_settings();
 }if (strcmp(schluessel, "zz") == 0){
 zeitverschiebung= strtoul(parameter, NULL, 10);
 get_ntp_time();
  }if (strcmp(schluessel, "nth_en") == 0){
 nthen= strtoul(parameter, NULL, 10);
 }if (strcmp(schluessel, "nth") == 0){
 nth= strtoul(parameter, NULL, 10);
 }if (strcmp(schluessel, "nthe") == 0){
 nthe= strtoul(parameter, NULL, 10);
    }if (strcmp(schluessel, "nthca") == 0){
nthr = value1;
nthg = value2;
nthb = value0;
     }if (strcmp(schluessel, "nthco") == 0){ //off
nthzr = value1;
nthzg = value2;
nthzb = value0;
   }if (strcmp(schluessel, "mode") == 0){
 ledmode= strtoul(parameter, NULL, 10);
      }if (strcmp(schluessel, "conh") == 0){ //off
hzr = value1;
hzg = value2;
hzb = value0;
her = value1;
heg = value2;
heb = value0;
       }if (strcmp(schluessel, "conm") == 0){ //off
mzr = value1;
mzg =value2;
mzb = value0;
mer = value1;
meg = value2;
meb = value0;
       }if (strcmp(schluessel, "cons") == 0){ //off
szr = value1;
szg = value2;
szb = value0;
ser = value1;
seg = value2;
seb = value0;
       }if (strcmp(schluessel, "coh") == 0){ //off
hzrz = value1;
hzgz = value2;
hzbz = value0;
herz = value1;
hegz = value2;
hebz = value0;
       }if (strcmp(schluessel, "com") == 0){ //off
mzrz = value1;
mzgz = value2;
mzbz = value0;
merz = value1;
megz = value2;
mebz =value0;
       }if (strcmp(schluessel, "cos") == 0){ //off
szrz = value1;
szgz = value2;
szbz =value0;
serz = value1;
segz = value2;
sebz =value0;

  }if (strcmp(schluessel, "stopalarm") == 0){ //off
stop_alarm();
 }if (strcmp(schluessel, "getntp") == 0){ //off
 get_ntp_time();;
 }if (strcmp(schluessel, "getgps") == 0){ //off
 get_gps_time();


}



 save_settings(); 
 load_settings();
}



 
      
      
      
      
      
      
      
      
      
      
      
      
      
      void start_webui(){
  webserver.setDefaultCommand(&defaultCmd);
   webserver.setFailureCommand(&defaultCmd); // 404
  webserver.addCommand("index.html", &defaultCmd);
  webserver.addCommand("alarm.html", &alarmCmd);
   webserver.addCommand("general.html", &generalCmd);
      webserver.addCommand("night.html", &nightCmd);
      webserver.addCommand("color.html", &colorCmd);
    //  webserver.addCommand("network.html", &networkCmd);
  webserver.begin();
}
      
