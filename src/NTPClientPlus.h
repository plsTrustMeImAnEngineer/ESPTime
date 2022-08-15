#ifndef NTPClientPlus_h
#define NTPClientPlus_h

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <Arduino.h>
#include <time.h>
#include <WifiUdp.h>
#include <Udp.h>

#define SEVENZYYEARS 2208988800UL
// #define NTP_PACKET_SIZE 48
const int NTP_PACKET_SIZE PROGMEM = 48;

class NTPClientPlus
{
    private:
    UDP* udp;
    bool udpSetup = false;
    uint16_t port = 1337;
    int timeOffset;
    uint16_t updateInterval;  // In ms
    uint32_t unixtime = 0;      // In s
    uint32_t lastUpdate = 0;      // In ms
    bool summertime = true;
    bool considerSummertime = true;
    const char* poolServerName;
    uint8_t packetBuffer[NTP_PACKET_SIZE];

    void sendNTPPacket();
    
    uint8_t getRawHour() const;
    uint8_t getRawMonthDay() const;
    uint8_t getRawMonth() const;
    uint16_t getRawYear() const;
    

    public:
    NTPClientPlus(int timeOffset = 0, int updateInterval = 60000, const char* poolServerName = "pool.ntp.org") :
        timeOffset(timeOffset),
        updateInterval(updateInterval),
        poolServerName(poolServerName)
    {udp = new WiFiUDP;}

    NTPClientPlus(UDP& udp, int timeOffset = 0, int updateInterval = 60000, const char* poolServerName = "pool.ntp.org") :
        udp(&udp),
        timeOffset(timeOffset),
        updateInterval(updateInterval),
        poolServerName(poolServerName)
    {}

    ~NTPClientPlus() {delete udp;}

    void begin(int port);
    void end();
    bool update();
    bool forceUpdate();
    
    void setPoolServer(const char* poolServerName);
    void setTimeOffset(int timeOffset);
    void setUpdateInterval(int updateInterval);

    uint8_t getDay();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();
    uint8_t getMonthDay();
    uint8_t getMonth();
    uint16_t getYear();
    
    String getFormattedTime();
    String getFullFormattedTime();
    uint32_t getUnixTime() const;

    void setConsiderSummertime(bool considerSummertime);
    bool getConsiderSummertime() const;
    bool isSummertime();
};

#endif