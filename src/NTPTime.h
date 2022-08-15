#ifndef NTPTime_h
#define NTPTime_h

#include <ESPTime.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#else
#include <WiFi.h>
#endif




class NTPTime : public ESPTime
{
    private:
    static constexpr uint8_t PROGMEM packetBufferSize = 48;
    static constexpr time_t PROGMEM seventyYears = 2208988800;

    UDP* udp;
    bool udpSetUp = false;
    const char* ntpServer;
    uint32_t updateInterval; // in in ms
    uint32_t lastUpdate; // in ms
    uint8_t packetBuffer[packetBufferSize];

    void sendNTPPacket();

    public:
    NTPTime(const char* timezone = "UTC0", uint32_t updateInterval = 60000, const char* ntpServer = "pool.ntp.org");
    NTPTime(UDP& udp, const char* timezone = "UTC0", uint32_t updateInterval = 60000, const char* ntpServer = "pool.ntp.org");
    ~NTPTime();

    void begin(uint16_t port = 1337);
    void end();
    bool update();
    bool forceUpdate();
    void setServer(const char* ntpServer);
    const char* getServer() const;
    void setUpdateInterval(uint32_t updateInterval);
    uint32_t getUpdateInterval() const;
};

#endif