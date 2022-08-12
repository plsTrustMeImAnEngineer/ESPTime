#include <Arduino.h>
#include <NTPClientPlus.h>

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

// configure NTPClient with time offset 1h (to configure timezone)
NTPClientPlus ntp(3600);

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
}

void loop()
{
  // update NTPClient periodically
  ntp.update();
  
  // print formatted date and time
  Serial.println(ntp.getFullFormattedTime());
  delay(1000);
}