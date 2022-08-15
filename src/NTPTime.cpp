#include <NTPTime.h>

NTPTime::NTPTime(const char* timezone, uint32_t updateInterval, const char* ntpServer)
{
    this->timezone = timezone;
    this->updateInterval = updateInterval;
    this->ntpServer = ntpServer;
    udp = new WiFiUDP;
}

NTPTime::NTPTime(UDP& udp, const char* timezone, uint32_t updateInterval, const char* ntpServer)
{
    this->udp = &udp;
    this->timezone = timezone;
    this->updateInterval = updateInterval;
    this->ntpServer = ntpServer;
}

NTPTime::~NTPTime()
{
    end();
}

void NTPTime::begin(uint16_t port)
{
    udp->begin(port);
    udpSetUp = true;
}

void NTPTime::end()
{
    udp->stop();
    delete udp;
}

bool NTPTime::update()
{
    if ((millis() - lastUpdate >= updateInterval) || lastUpdate == 0)
    {
        if(!udpSetUp)
            begin();
        return forceUpdate();
    }
    return true;
}

bool NTPTime::forceUpdate()
{
    
}