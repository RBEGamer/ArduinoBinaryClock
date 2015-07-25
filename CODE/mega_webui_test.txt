/* Web_Authentication.ino - Webduino Authentication example */

/* This example assumes that you're familiar with the basics
 * of the Ethernet library (particularly with setting MAC and
 * IP addresses) and with the basics of Webduino. If you
 * haven't had a look at the HelloWorld example you should
 * probably check it out first */

/* you can change the authentication realm by defining
 * WEBDUINO_AUTH_REALM before including WebServer.h */
#define WEBDUINO_AUTH_REALM "Weduino Authentication Example"

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
 * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
 * that's not in use and isn't going to be automatically allocated by
 * DHCP from your router. */
static uint8_t ip[] = { 192, 168, 178, 58 };


 template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }

int hour = 14, minute = 9, second = 59;
int alarm_en = 0, alarm_h = 16, alarm_m = 6;
int alarm_button_enable = 0, zeitverschiebung = 0;

int nthen = 0;
int nth = 22, nthe = 6;
int nthr = 0, nthg = 2, nthb = 0;
int nthzr = 0, nthzg = 0, nthzb = 1;
int ledmode = 0;
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

//NEW VARS
char *webui_credentials = "YWRtaW46MTU5MA==";// admin:1590 //http://www.base64encode.org/
#define PREFIX ""
WebServer webserver(PREFIX, 80);
#define NAMELEN 64
#define VALUELEN 64
const int post_buffer_size = 64;




    P(head) = "<html><head>"
"<meta http-equiv='Content-Type' content='text/html; charset=windows-1252'>"
"<title>EPIC NERD CLOCK ADMIN PAGE</title>"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'/>"
"<meta name='apple-mobile-web-app-capable' content='yes'/>"
"<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'/>"
"<link rel='stylesheet' href='http://clock.rbnetworks.de/chromoselector.css'/>"
"<style type='text/css'>a:link{color:#FFF;}a:visited{color:#FFF;}a:hover{color:#FFCA00;}a:active{color:#FFAA00;}</style>"
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
"<td><a href='/general.html'>TWITTER SETTINGS</a></td>"
"<td><a href='/network.html'>NETWORK SETTINGS</a></td>"
"<td><a href='/general.html'>GENERAL SETTINGS</a></td>"
"</tr></table>"
"<p style='margin-left: 20'><font face='Arial' size='2'>&nbsp;</font></p>";
P(page_end) = "<table border='0' width='100%' background='http://clock.rbnetworks.de/img/botbkg.gif' cellspacing='0' cellpadding='0'>"
   "<tr>"
"<td width='100%'><img border='0' src='http://clock.rbnetworks.de/img/botbkg.gif' width='38' height='14'></td>"
"</tr>"
"</table>"
   "</center></body></html>";
   
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


void get_ntp_time(){}
void restore_settings(){}




  
      //hzr = getValue(parameter, ',', 1).toInt();
 void process_response(char schluessel[256], char parameter[256]){
 Serial.print("POST : "); Serial.print(schluessel ); Serial.print("=");Serial.println(parameter);
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
nthr = getValue(parameter, ',', 0).toInt();
nthg = getValue(parameter, ',', 1).toInt();
nthb = getValue(parameter, ',', 2).toInt();
     }if (strcmp(schluessel, "nthco") == 0){ //off
nthzr = getValue(parameter, ',', 0).toInt();
nthzg = getValue(parameter, ',', 1).toInt();
nthzb = getValue(parameter, ',', 2).toInt();
   }if (strcmp(schluessel, "mode") == 0){
 ledmode= strtoul(parameter, NULL, 10);
      }if (strcmp(schluessel, "conh") == 0){ //off
hzr = getValue(parameter, ',', 0).toInt();
hzg = getValue(parameter, ',', 1).toInt();
hzb = getValue(parameter, ',', 2).toInt();
her = getValue(parameter, ',', 0).toInt();
heg = getValue(parameter, ',', 1).toInt();
heb = getValue(parameter, ',', 2).toInt();
       }if (strcmp(schluessel, "conm") == 0){ //off
mzr = getValue(parameter, ',', 0).toInt();
mzg = getValue(parameter, ',', 1).toInt();
mzb = getValue(parameter, ',', 2).toInt();
mer = getValue(parameter, ',', 0).toInt();
meg = getValue(parameter, ',', 1).toInt();
meb = getValue(parameter, ',', 2).toInt();
       }if (strcmp(schluessel, "cons") == 0){ //off
szr = getValue(parameter, ',', 0).toInt();
szg = getValue(parameter, ',', 1).toInt();
szb = getValue(parameter, ',', 2).toInt();
ser = getValue(parameter, ',', 0).toInt();
seg = getValue(parameter, ',', 1).toInt();
seb = getValue(parameter, ',', 2).toInt();
       }if (strcmp(schluessel, "coh") == 0){ //off
hzrz = getValue(parameter, ',', 0).toInt();
hzgz = getValue(parameter, ',', 1).toInt();
hzbz = getValue(parameter, ',', 2).toInt();
herz = getValue(parameter, ',', 0).toInt();
hegz = getValue(parameter, ',', 1).toInt();
hebz = getValue(parameter, ',', 2).toInt();
       }if (strcmp(schluessel, "com") == 0){ //off
mzrz = getValue(parameter, ',', 0).toInt();
mzgz = getValue(parameter, ',', 1).toInt();
mzbz = getValue(parameter, ',', 2).toInt();
merz = getValue(parameter, ',', 0).toInt();
megz = getValue(parameter, ',', 1).toInt();
mebz = getValue(parameter, ',', 2).toInt();
       }if (strcmp(schluessel, "cos") == 0){ //off
szrz = getValue(parameter, ',', 0).toInt();
szgz = getValue(parameter, ',', 1).toInt();
szbz = getValue(parameter, ',', 2).toInt();
serz = getValue(parameter, ',', 0).toInt();
segz = getValue(parameter, ',', 1).toInt();
sebz = getValue(parameter, ',', 2).toInt();
  
}



// save_settings(); 
// load_settings();
}






  





void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete){
     URLPARAM_RESULT rc;
      char name[NAMELEN];char value[VALUELEN];
if (server.checkCredentials(webui_credentials)){ 
  server.httpSuccess();
  if (type != WebServer::HEAD)
  { 
     server.printP(head);
     server.printP(page_start); 
 //_-----------------------------    
server<<"<div><h2> CURRENT TIME: ";if(hour < 10){server<< "0"<< hour;}else{server<<hour;} server<<":";if(minute < 10){server<< "0"<< minute;}else{server<<minute;}server<<":";if(second < 10){server<< "0"<< second;}else{server<<second;} server<<"</h2></div>";
server<<"<p style='margin-left: 20'><font face='Arial' size='2'>&nbsp;</font></p>";     
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
     process_response(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 process_response(name,value);
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
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
     process_response(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 process_response(name,value);
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
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
     process_response(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 process_response(name,value);
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
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
     process_response(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 process_response(name,value);
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
}else{
server.httpUnauthorized();
}
if (strlen(url_tail)){
    while (strlen(url_tail)){
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS){
     process_response(name,value);
      }else{}
      }
    }
  if (type == WebServer::POST){
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {   
 process_response(name,value);
    }
  } 
}










void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  webserver.setDefaultCommand(&defaultCmd);
   webserver.setFailureCommand(&defaultCmd); // 404
  webserver.addCommand("index.html", &defaultCmd);
  webserver.addCommand("alarm.html", &alarmCmd);
   webserver.addCommand("general.html", &generalCmd);
      webserver.addCommand("night.html", &nightCmd);
      webserver.addCommand("color.html", &colorCmd);
  webserver.begin();
}

void loop()
{
  char buff[post_buffer_size];
  int len = post_buffer_size;
  webserver.processConnection(buff, &len);
}