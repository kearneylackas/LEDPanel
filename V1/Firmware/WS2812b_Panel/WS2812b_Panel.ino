#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "FastLED.h"

/*_______________________________________________________________________________________________*/

#define NUM_LEDS_PER_STRIP 25
#define NUM_STRIPS 6
#define NUM_LEDS 150
CRGB LEDStrip[NUM_STRIPS][NUM_LEDS_PER_STRIP];
//const unsigned int LEDStripPin[6] = {4, 5, 14, 15, 13, 12};

/*_______________________________________________________________________________________________*/

#define MAX_SRV_CLIENTS 5
#define HEADER_SIZE 3
#define FOOTER_SIZE 1
#define BUFFER_SIZE 450
const char* ssid = "K&K (2.4GHz)";
const char* password = "kearney30";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];
uint8_t buff[BUFFER_SIZE];

/*_______________________________________________________________________________________________*/

unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp;
unsigned long epoch;
unsigned long lastNTPSend = 0;
unsigned long nextNTPSend = 0;
unsigned long nextNTPRead = 0;
bool packetSent;

/*_______________________________________________________________________________________________*/

// led information {strip number, x coordinate (mils), y coordinate (mils), first level neighbors (x3), 2nd level neighbors (x6)}
int LEDData[NUM_STRIPS * NUM_LEDS_PER_STRIP][5] =
{ {0, 1312, -757, 330, 1515},
  {0, 2624, -1515, 330, 3030},
  {0, 3937, -757, 349, 4009},
  {0, 5249, -1515, 344, 5463},
  {0, 6562, -757, 353, 6606},
  {0, 7874, -1515, 349, 8018},
  {0, 9187, -757, 355, 9218},
  {0, 10499, -1515, 352, 10608},
  {0, 11812, -757, 356, 11836},
  {0, 10499, -3031, 344, 10928},
  {0, 9187, -3788, 338, 9937},
  {0, 7874, -3031, 339, 8437},
  {0, 6562, -3788, 330, 7577},
  {0, 5249, -3031, 330, 6061},
  {0, 3937, -3788, 316, 5463},
  {0, 2624, -3031, 311, 4009},
  {0, 3937, -5304, 307, 6605},
  {0, 5249, -6062, 311, 8019},
  {0, 6562, -5304, 321, 8438},
  {0, 7874, -6062, 322, 9937},
  {0, 9187, -5304, 330, 10608},
  {0, 7874, -7577, 316, 10928},
  {0, 6562, -8335, 308, 10608},
  {0, 5249, -7577, 305, 9218},
  {0, 6562, -9851, 304, 11836},
  {1, 0, -1515, 270, 1515},
  {1, 0, -3030, 270, 3030},
  {1, 1313, -3788, 289, 4009},
  {1, 1312, -5303, 284, 5463},
  {1, 2625, -6062, 293, 6606},
  {1, 2625, -7577, 289, 8019},
  {1, 3938, -8335, 295, 9218},
  {1, 3937, -9850, 292, 10608},
  {1, 5250, -10608, 296, 11836},
  {1, 2625, -10608, 284, 10928},
  {1, 1313, -9850, 278, 9937},
  {1, 1312, -8334, 279, 8437},
  {1, 0, -7577, 270, 7577},
  {1, 0, -6061, 270, 6061},
  {1, -1312, -5304, 256, 5464},
  {1, -1313, -3788, 251, 4009},
  {1, -2625, -6062, 247, 6606},
  {1, -2625, -7577, 251, 8019},
  {1, -1313, -8335, 261, 8438},
  {1, -1312, -9850, 262, 9937},
  {1, 0, -10608, 270, 10608},
  {1, -2625, -10608, 256, 10928},
  {1, -3937, -9850, 248, 10608},
  {1, -3938, -8334, 245, 9218},
  {1, -5250, -10608, 244, 11836},
  {2, -1312, -757, 210, 1515},
  {2, -2624, -1515, 210, 3030},
  {2, -2624, -3031, 229, 4009},
  {2, -3937, -3788, 224, 5463},
  {2, -3937, -5304, 233, 6605},
  {2, -5249, -6062, 229, 8019},
  {2, -5249, -7577, 235, 9218},
  {2, -6562, -8335, 232, 10608},
  {2, -6562, -9851, 236, 11836},
  {2, -7874, -7577, 224, 10928},
  {2, -7874, -6062, 218, 9937},
  {2, -6562, -5304, 219, 8438},
  {2, -6562, -3789, 210, 7577},
  {2, -5249, -3030, 210, 6061},
  {2, -5249, -1515, 196, 5463},
  {2, -3937, -757, 191, 4009},
  {2, -6562, -757, 187, 6606},
  {2, -7874, -1515, 191, 8018},
  {2, -7875, -3031, 201, 8438},
  {2, -9186, -3788, 202, 9936},
  {2, -9187, -5304, 210, 10608},
  {2, -10499, -3030, 196, 10927},
  {2, -10499, -1515, 188, 10608},
  {2, -9187, -757, 185, 9218},
  {2, -11812, -757, 184, 11836},
  {3, -1312, 757, 150, 1515},
  {3, -2624, 1515, 150, 3030},
  {3, -3937, 757, 169, 4009},
  {3, -5249, 1515, 164, 5463},
  {3, -6562, 757, 173, 6606},
  {3, -7874, 1515, 169, 8018},
  {3, -9187, 757, 175, 9218},
  {3, -10499, 1515, 172, 10608},
  {3, -11812, 757, 176, 11836},
  {3, -10499, 3031, 164, 10928},
  {3, -9187, 3788, 158, 9937},
  {3, -7874, 3031, 159, 8437},
  {3, -6562, 3788, 150, 7577},
  {3, -5249, 3031, 150, 6061},
  {3, -3937, 3788, 136, 5463},
  {3, -2624, 3031, 131, 4009},
  {3, -3937, 5304, 127, 6605},
  {3, -5249, 6062, 131, 8019},
  {3, -6562, 5304, 141, 8438},
  {3, -7874, 6062, 142, 9937},
  {3, -9187, 5304, 150, 10608},
  {3, -7874, 7577, 136, 10928},
  {3, -6562, 8335, 128, 10608},
  {3, -5249, 7577, 125, 9218},
  {3, -6562, 9851, 124, 11836},
  {4, 0, 1515, 90, 1515},
  {4, 0, 3030, 90, 3030},
  {4, -1313, 3788, 109, 4009},
  {4, -1312, 5303, 104, 5463},
  {4, -2625, 6062, 113, 6606},
  {4, -2625, 7577, 109, 8019},
  {4, -3938, 8335, 115, 9218},
  {4, -3937, 9850, 112, 10608},
  {4, -5250, 10608, 116, 11836},
  {4, -2625, 10608, 104, 10928},
  {4, -1313, 9850, 98, 9937},
  {4, -1312, 8334, 99, 8437},
  {4, 0, 7577, 90, 7577},
  {4, 0, 6061, 90, 6061},
  {4, 1312, 5304, 76, 5464},
  {4, 1313, 3788, 71, 4009},
  {4, 2625, 6062, 67, 6606},
  {4, 2625, 7577, 71, 8019},
  {4, 1313, 8335, 81, 8438},
  {4, 1312, 9850, 82, 9937},
  {4, 0, 10608, 90, 10608},
  {4, 2625, 10608, 76, 10928},
  {4, 3937, 9850, 68, 10608},
  {4, 3938, 8334, 65, 9218},
  {4, 5250, 10608, 64, 11836},
  {5, 1312, 757, 30, 1515},
  {5, 2624, 1515, 30, 3030},
  {5, 2624, 3031, 49, 4009},
  {5, 3937, 3788, 44, 5463},
  {5, 3937, 5304, 53, 6605},
  {5, 5249, 6062, 49, 8019},
  {5, 5249, 7577, 55, 9218},
  {5, 6562, 8335, 52, 10608},
  {5, 6562, 9851, 56, 11836},
  {5, 7874, 7577, 44, 10928},
  {5, 7874, 6062, 38, 9937},
  {5, 6562, 5304, 39, 8438},
  {5, 6562, 3789, 30, 7577},
  {5, 5249, 3030, 30, 6061},
  {5, 5249, 1515, 16, 5463},
  {5, 3937, 757, 11, 4009},
  {5, 6562, 757, 7, 6606},
  {5, 7874, 1515, 11, 8018},
  {5, 7875, 3031, 21, 8438},
  {5, 9186, 3788, 22, 9936},
  {5, 9187, 5304, 30, 10608},
  {5, 10499, 3030, 16, 10927},
  {5, 10499, 1515, 8, 10608},
  {5, 9187, 757, 5, 9218},
  {5, 11812, 757, 4, 11836}
};

/*_______________________________________________________________________________________________*/

bool writeReceivedData() {
  byte red, green, blue;
  int start;
  for (int currentLED = 0; currentLED < NUM_LEDS; currentLED++) {
    start = 3 * currentLED;
    red   = buff[start + 0];
    green = buff[start + 1];
    blue  = buff[start + 2];
    buff[start + 0] = 0;
    buff[start + 1] = 0;
    buff[start + 2] = 0;

    int currentLEDStrip = LEDData[currentLED][0];
    int currentLEDLocal = currentLED % 25;
    LEDStrip[currentLEDStrip][currentLEDLocal].setRGB(red, green, blue);
  }
  FastLED.show();
}

/*_______________________________________________________________________________________________*/

void radialRainbow(int msDelay) {
  unsigned long startTime = millis();
  int currentLEDHue = 0;
  int currentLEDStrip = 0;
  int currentLEDLocal = 0;
  bool debug = false;
  for (int offset = 0; offset < 256; offset += 1) {
    for (int currentLED = 0; currentLED < 150; currentLED++) {
      currentLEDHue = (int)(LEDData[currentLED][3] * 0.7111 + offset) % 256;
      currentLEDStrip = LEDData[currentLED][0];
      currentLEDLocal = currentLED % 25;

      if (debug) {
        Serial.print("STRIP: ");
        Serial.print(currentLEDStrip);
        Serial.print("\t\t");
        Serial.print("NUMBER: ");
        Serial.print(currentLEDLocal);
        Serial.print("\t\t");
        Serial.print("VALUE: ");
        Serial.print(currentLEDHue);
        Serial.println("");
      }
      LEDStrip[currentLEDStrip][currentLEDLocal] = CHSV(currentLEDHue, 255, 127);
    }
    FastLED.show();
    delay(msDelay);
  }

  unsigned long deltaTime = millis() - startTime;
  unsigned long hertz = 256000. / deltaTime;
  Serial.print("Refresh Rate: ");
  Serial.println(hertz);
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress & address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void printUTCTime( unsigned long seconds) {
  // print the hour, minute and second:
  Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
  Serial.print((seconds % 86400L) / 3600); // print the hour (86400 equals secs per day)
  Serial.print(':');
  if ( ((seconds % 3600) / 60) < 10 ) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.print((seconds  % 3600) / 60); // print the minute (3600 equals secs per minute)
  Serial.print(':');
  if ( (seconds % 60) < 10 ) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.println(seconds % 60); // print the second
  Serial.println("");
}

unsigned long parseNTPPacket() {
  udp.read(packetBuffer, NTP_PACKET_SIZE);

  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  unsigned long secsSince1900 = highWord << 16 | lowWord;

  Serial.print("Seconds since Jan 1 1900 = " );
  Serial.println(secsSince1900);

  Serial.print("Unix time = ");
  const unsigned long seventyYears = 2208988800UL;
  epoch = secsSince1900 - seventyYears;
  Serial.println(epoch);
}

bool updateTelnet() {
  uint8_t i, j;
  bool newData = false;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        Serial.println(serverClients[i].available());
        serverClients[i].read(buff, serverClients[i].available());
        //while (serverClients[i].available() < 2) delay(1);
        //serverClients[i].flush();
        newData = true;
      }
    }
  }
  return newData;
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if (i == 21) {
    Serial.print("Could not connect to"); Serial.println(ssid);
    while (1) delay(500);
  }

  server.begin();
  server.setNoDelay(true);
  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  FastLED.addLeds<NEOPIXEL, 4>(LEDStrip[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(LEDStrip[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 14>(LEDStrip[2], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 15>(LEDStrip[3], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 13>(LEDStrip[4], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 12>(LEDStrip[5], NUM_LEDS_PER_STRIP);
  FastLED.show();
}

void loop() {
  if (updateTelnet()) {
    writeReceivedData();
    buff[BUFFER_SIZE] = {};
  }

  if (!packetSent && millis() > nextNTPSend) {
    WiFi.hostByName(ntpServerName, timeServerIP);
    sendNTPpacket(timeServerIP);
    lastNTPSend = millis();
    nextNTPRead = lastNTPSend + 1000;
    packetSent = true;
  }

  if (packetSent && millis() > nextNTPRead) {
    int cb = udp.parsePacket();
    if (cb) {
      Serial.print("Packet Received, Length = ");
      Serial.println(cb);
      parseNTPPacket();
    } else {
      Serial.println("No Packet Received");
      nextNTPRead = 0 - 1;
    }
    packetSent = false;
    nextNTPSend = lastNTPSend + 60000;
  }

  printUTCTime(epoch + (millis() - lastNTPSend) / 1000);

  radialRainbow(0);
}

