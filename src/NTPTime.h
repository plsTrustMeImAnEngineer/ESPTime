#ifndef NTPTime_h
#define NTPTime_h

#include <ESPTime.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif


class NTPTime : public ESPTime
{
    private:
    const char* ntpServer;
    uint32_t updateInterval; // in in ms

    public:
    NTPTime(const char* timezone = TZ_Etc_UTC, uint32_t updateInterval = 60000, const char* ntpServer = "pool.ntp.org");

    void setServer(const char* ntpServer);
    const char* getServer() const;

    void setUpdateInterval(uint32_t updateInterval);
    uint32_t getUpdateInterval() const;

    bool update();
    void forceUpdate();

};

#endif