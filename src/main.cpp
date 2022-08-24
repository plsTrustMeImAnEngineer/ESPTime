#include <Arduino.h>
#include <ESPSystemTime.h>

ESPSystemTime systemTime(TZ_Europe_Berlin);


void setup()
{
    Serial.begin(9600);

    WiFi.begin("WLAN-T-Com", "GxvuzR8znV");

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    systemTime.setUpdateInterval(5000);
}

void loop()
{   
    if(systemTime.update()) Serial.println("UPDATE");
    Serial.println(systemTime.getFormattedDateTime());
    Serial.println(systemTime.getUnixTime());
    delay(1000);
}