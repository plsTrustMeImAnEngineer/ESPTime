#include <Arduino.h>
#include <NTPClientPlus.h>

#define timeOffset 3600
#define updateInterval 30000

const char* ssid = "WLAN-T-Com";
const char* password = "GxvuzR8znV";

WiFiUDP udp;
// configure NTPClient
NTPClientPlus ntp(udp, timeOffset, updateInterval, "de.pool.ntp.org");

void setup()
{
  Serial.begin(9600);

  // begin WiFi connection
  WiFi.begin(ssid, password);

  // wait for WiFi to connect
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  ntp.setConsiderSummertime(false); // disable Summertime
}

void loop()
{
  // update NTPClient periodically
  ntp.update();

  //print Time
  Serial.println(ntp.getFullFormattedTime());
  delay(1000);
}