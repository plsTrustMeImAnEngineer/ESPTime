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
    _updateInterval = updateInterval;
    _ntpSync = ntpSync;
    _ntpServer = ntpServer;

    if(ntpSync) forceUpdate();
}

/**
 * @brief Sets the NTP-Server
 * 
 * @param ntpServer The url of the NTP-Server
 */
void ESPSystemTime::setServer(const char* ntpServer)
{
    _ntpServer = ntpServer;
}

/**
 * @return The url of the NTP-Server 
 */
const char* ESPSystemTime::getServer() const
{
    return _ntpServer;
}

/**
 * @brief Periodically updates the time after updateInterval elapsed
 * 
 * @return true, if an update actually happend
 * @return false, if updateInterval hasn't elapsed yet
 */
bool ESPSystemTime::update()
{
    time(&_unixtime);
    static uint32_t lastUpdate = 0;
    if((millis() - lastUpdate >= _updateInterval) || lastUpdate == 0)
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
    if(_ntpSync) configTzTime(_timezone, _ntpServer);
    time(&_unixtime);
}

/**
 * @brief Sets the update interval in milliseconds
 * 
 * @param updateInterval 
 */
void ESPSystemTime::setUpdateInterval(uint32_t updateInterval)
{
    _updateInterval = updateInterval;
}

/** 
 * @return The update interval in milliseconds 
 */
uint32_t ESPSystemTime::getUpdateInterval() const
{
    return _updateInterval;
}

/**
 * @brief Sets whether the time is synced over NTP
 * 
 * @param isActive 
 */
void ESPSystemTime::setNTPSync(bool isActive)
{
    _ntpSync = isActive;
}

/**
 * @return true, if time gets synced over NTP
 */
bool ESPSystemTime::isNTPSync() const
{
    return _ntpSync;
}