#include <ESPSystemTime.h>

ESPSystemTime::ESPSystemTime(const char* timezone, uint32_t updateInterval, bool ntpSync, const char* ntpServer)
{
    setTimezone(timezone);
    this->updateInterval = updateInterval;
    this->ntpSync = ntpSync;
    this->ntpServer = ntpServer;

    if(ntpSync) forceUpdate();
}

void ESPSystemTime::setServer(const char* ntpServer)
{
    this->ntpServer = ntpServer;
}

const char* ESPSystemTime::getServer() const
{
    return ntpServer;
}

bool ESPSystemTime::update()
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

void ESPSystemTime::forceUpdate()
{
    if(ntpSync) configTzTime(timezone, ntpServer);
    time(&unixtime);
}

void ESPSystemTime::setUpdateInterval(uint32_t updateInterval)
{
    this->updateInterval = updateInterval;
}

uint32_t ESPSystemTime::getUpdateInterval() const
{
    return updateInterval;
}

void ESPSystemTime::setNTPSync(bool isActive)
{
    ntpSync = isActive;
}

bool ESPSystemTime::isNTPSync() const
{
    return ntpSync;
}