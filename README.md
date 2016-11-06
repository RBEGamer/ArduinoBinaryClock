# ArduinoBinaryClock
Arduino based binary clock with Webinteface, GPS/NTP-Sync, Alarm, Timezones


# FEATURES
* Timesync via GPS or NTP-Server
* Timezone support
* Alarm
* Nightmode (dim leds)
* Colorcycle, custom colors set via webinterface

# PARTS
* Arduino Mega 2560 (the flash of the uno is to small)
* Wiz-Ethernetshield
* Serial GPS Module (optional)
* 24 WS2812 5mm LEDs
* MiniSpeaker for alarm
* 3 Switches
* acrylic glass for the case, but you can take any other material

# TOOLS
* soldering equipment
* dril 5mm, 6mm (for switches)
* saw
* hotglue

# SETUP HARDWARE (simplyfied version)
* solder all leds to a long chain,  connect DOUT to DIN of the next led
* connect the TRC Module to the I2C Bus of the Arduino
* connect the buttons with pullup resistors to D4, D5, D6 (if pressed pin to ground)
* connect the DI of the first led with pin D7 of the arduino
* connect the speaker to PIN D2
* drill 5mm holes into your front plate of the case
* glue the leds in
* place the other parts inside the case 

# SETUP SOFTWARE
* download the latest Arduino IDE
* download the sketch located at /src/mega_beta14_ gps vorbereitet/
* upload it to your arduino 

# USE IT
* connect the clock via ethernet with your local network
* connect usb to power the clock up
* get the ip from your clock via your router/dhcp server
* login to the webinterface with:

## Credentials
* USERNAME : admin
* PASSWORD : 1590

# IMAGES
![Gopher image](PHOTOS/clock_open.JPG)
