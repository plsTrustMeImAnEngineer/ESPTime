#include <Arduino.h>
#include <NTPTime.h>

const char* ssid = "WLAN-T-Com";
const char* password = "GxvuzR8znV";

NTPTime ntp(TZ_Europe_Berlin);

void setup()
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    ntp.setUpdateInterval(30000);
}

void loop()
{   
    if(ntp.update()) Serial.println ("UPDATE");
    Serial.println(ntp.getFormattedDateTime());
    Serial.println(ntp.getUnixTime());
    delay(1000);
}