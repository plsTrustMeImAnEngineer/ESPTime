#ifndef NTPTime_h
#define NTPTime_h

#include <ESPTime.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else 
#warning Library incompatible. Use ESP32 or ESP8266
#endif


class ESPSystemTime : public ESPTime
{
    private:
    const char* _ntpServer;
    bool _ntpSync = true;
    uint32_t _updateInterval; // in in ms

    public:
    ESPSystemTime(const char* timezone = TZ_Etc_UTC, uint32_t updateInterval = 60000, bool ntpSync = true, const char* ntpServer = "pool.ntp.org");

    void setServer(const char* ntpServer);
    const char* getServer() const;

    void setUpdateInterval(uint32_t updateInterval);
    uint32_t getUpdateInterval() const;

    bool update();
    void forceUpdate();

    void setNTPSync(bool isA);
    bool isNTPSync() const;
};

#endif