#include <ESPSystemTime.h>

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

const uint32_t updateInterval = 30000; // in ms

ESPSystemTime systemTime(TZ_Europe_Amsterdam, updateInterval);

void setup()
{
    Serial.begin(9600);

    // Connect Wifi in order to update time from NTP Server
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
}

void loop()
{
    systemTime.update(); // call this method to automatically update time
    Serial.println(systemTime.getFormattedDateTime());
}