#include <ESPSystemTime.h>

/**
 * @brief Construct a new ESPSystemTime::ESPSystemTime object
 * 
 * @param timezone The timezone-string (Recommendation: Use macro from Timezones.h)
 * @param updateInterval The update interval in milliseconds
 * @param ntpSync if true, time will be updated over NTP
 * @param ntpServer The of the NTP-Server
 */
ESPSystemTime::ESPSystemTime(const char* timezone, uint32_t updateInterval, bool ntpSync, const char* ntpServer)
{
    setTimezone(timezone);
    this->updateInterval = updateInterval;
    this->ntpSync = ntpSync;
    this->ntpServer = ntpServer;

    if(ntpSync) forceUpdate();
}

/**
 * @brief Sets the NTP-Server
 * 
 * @param ntpServer The url of the NTP-Server
 */
void ESPSystemTime::setServer(const char* ntpServer)
{
    this->ntpServer = ntpServer;
}

/**
 * @return The url of the NTP-Server 
 */
const char* ESPSystemTime::getServer() const
{
    return ntpServer;
}

/**
 * @brief Periodically updates the time after updateInterval elapsed
 * 
 * @return true, if an update actually happend
 * @return false, if updateInterval hasn't elapsed yet
 */
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

/**
 * @brief Forces the time to update, even if updateInterval hasn't elapsed yet
 */
void ESPSystemTime::forceUpdate()
{
    if(ntpSync) configTzTime(timezone, ntpServer);
    time(&unixtime);
}

/**
 * @brief Sets the update interval in milliseconds
 * 
 * @param updateInterval 
 */
void ESPSystemTime::setUpdateInterval(uint32_t updateInterval)
{
    this->updateInterval = updateInterval;
}

/** 
 * @return The update interval in milliseconds 
 */
uint32_t ESPSystemTime::getUpdateInterval() const
{
    return updateInterval;
}

/**
 * @brief Sets whether the time is synced over NTP
 * 
 * @param isActive 
 */
void ESPSystemTime::setNTPSync(bool isActive)
{
    ntpSync = isActive;
}

/**
 * @return true, if time gets synced over NTP
 */
bool ESPSystemTime::isNTPSync() const
{
    return ntpSync;
}