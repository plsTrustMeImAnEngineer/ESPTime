#include <NTPTime.h>

NTPTime::NTPTime(const char* timezone, uint32_t updateInterval, const char* ntpServer)
{
    setTimezone(timezone);
    this->updateInterval = updateInterval;
    this->ntpServer = ntpServer;
}

void NTPTime::setServer(const char* ntpServer)
{
    this->ntpServer = ntpServer;
}

const char* NTPTime::getServer() const
{
    return ntpServer;
}

bool NTPTime::update()
{
    time(&unixtime);
    static uint32_t lastUpdate = 0;
    if((millis() - lastUpdate >= updateInterval) || lastUpdate == 0)
    {
        forceUpdate();
        lastUpdate = millis();
        return true;
    }
    return false;
}

void NTPTime::forceUpdate()
{
    configTzTime(timezone, ntpServer);
    time(&unixtime);
}

void NTPTime::setUpdateInterval(uint32_t updateInterval)
{
    this->updateInterval = updateInterval;
}

uint32_t NTPTime::getUpdateInterval() const
{
    return updateInterval;
}