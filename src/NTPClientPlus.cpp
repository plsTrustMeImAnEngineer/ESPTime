#include "NTPClientPlus.h"

/**
 * @brief Starts the NTPClient. Usually you don't have to call this fuction explicitly.
 * update() and forceUpdate() do that for you
 * 
 * 
 * @param port Port for underlying UDP
 */
void NTPClientPlus::begin(int port = 1337)
{
    this->port = port;

    udp->begin(port);

    udpSetup = true;
}

/**
 * @brief Ends the underlying UDP connection
 */
void NTPClientPlus::end()
{
    udp->stop();
    udpSetup = false;
    delete udp;
}

/**
 * @brief This should be called in the main loop of your application. By default an update from the NTP Server is only
 * made every 60 seconds. This can be configured in the NTPClient constructor
 * 
 * @return true on success, false on failure 
 */
bool NTPClientPlus::update()
{
    if ((millis() - lastUpdate >= updateInterval) // Update after updateInterval
        || lastUpdate == 0)
    { // Update if there was no update yet.
        if (!udpSetup)
            begin(); // setup the UDP client if needed
        return forceUpdate();
    }
    return true;
}

/**
 * @brief Will force the NTPClient to update from the NTP Server
 * 
 * @return true on success, false on failure
 */
bool NTPClientPlus::forceUpdate()
{
#ifdef DEBUG_NTPClientPlus
    Serial.println("Update from NTP Server");
#endif
    if(!udpSetup) begin();

    sendNTPPacket();

    // Wait till data is there or timeout...
    byte timeout = 0;
    int cb = 0;
    do
    {
        delay(10);
        cb = udp->parsePacket();
        if (timeout > 100)
            return false; // timeout after 1000 ms
        timeout++;
    } while (cb == 0);

    lastUpdate = millis() - (10 * (timeout + 1)); // Account for delay in reading the time

    udp->read(packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    unixtime = secsSince1900 - SEVENZYYEARS;

    return true;
}

/**
 * @brief Set the NTP Pool Server
 * 
 * @param poolServerName The Name of the server
 */
void NTPClientPlus::setPoolServer(const char* poolServerName)
{
    this->poolServerName = poolServerName;
}

/**
 * @brief Changes the time offset. Useful for changing timezones dynamically
 * 
 * @param timeOffset Time offset in seconds
 */
void NTPClientPlus::setTimeOffset(int timeOffset)
{
    this->timeOffset = timeOffset;
}

/**
 * @brief Set the update interval to another frequency. The default update interval is 60 s
 * 
 * @param updateInterval New update interval in milliseconds
 */
void NTPClientPlus::setUpdateInterval(int updateInterval)
{
    this->updateInterval = updateInterval;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * @return current year
 */
uint16_t NTPClientPlus::getYear()
{
    time_t rawtime = getUnixTime();
    struct tm *ti;
    ti = localtime(&rawtime);
    int year = ti->tm_year + 1900;

    return year;
}

/**
 * @return current month
 */
uint8_t NTPClientPlus::getMonth()
{
    time_t rawtime = getUnixTime();
    struct tm *ti;
    ti = localtime(&rawtime);
    int month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);

    return month;
}

/**
 * @return the day of the month
 */
uint8_t NTPClientPlus::getMonthDay()
{
    time_t rawtime = getUnixTime();
    struct tm* ti;
    ti = localtime(&rawtime);
    int monthDay = ti->tm_hour;

    return monthDay;
}

/**
 * @return current weekday, 0 being Sunday 
 */
uint8_t NTPClientPlus::getDay() 
{
    return (((getUnixTime() / 86400L) + 4) % 7);
}

uint8_t NTPClientPlus::getHour()
{
    return ((getUnixTime() % 86400L) / 3600);
}

/**
 * @return current minute 
 */
uint8_t NTPClientPlus::getMinute() 
{
    return ((getUnixTime() % 3600) / 60);
}

/**
 * @return current second
 */
uint8_t NTPClientPlus::getSecond()
{
    return (getUnixTime() % 60);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * @return time formatted like 'hh:mm:ss'
 */
String NTPClientPlus::getFormattedTime()
{
    time_t offset = (considerSummertime && isSummertime()) ? 3600 : 0; 
    time_t rawtime = getUnixTime() + offset;
    struct tm* ti;
    ti = localtime(&rawtime);

    uint8_t hours = ti->tm_hour;
    String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

    uint8_t minutes = ti->tm_min;
    String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

    uint8_t seconds = ti->tm_sec;
    String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

    return hoursStr + ":" + minuteStr + ":" + secondStr;
}

/** 
 * @return  time formatted like 'yyyy-mm-ddd hh:mm:ss'
 */
String NTPClientPlus::getFullFormattedTime()
{
    // time_t offset = (considerSummertime && isSummertime()) ? 3600 : 0; 
    time_t rawtime = getUnixTime();
    struct tm* ti;
    ti = localtime(&rawtime);

    uint16_t year = ti->tm_year + 1900;
    String yearStr = String(year);

    uint8_t month = ti->tm_mon + 1;
    String monthStr = month < 10 ? "0" + String(month) : String(month);

    uint8_t day = ti->tm_mday;
    String dayStr = day < 10 ? "0" + String(day) : String(day);

    uint8_t hours = ti->tm_hour;
    String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

    uint8_t minutes = ti->tm_min;
    String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

    uint8_t seconds = ti->tm_sec;
    String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

    return yearStr + "-" + monthStr + "-" + dayStr + " " +
           hoursStr + ":" + minuteStr + ":" + secondStr;
}

/**
 * @brief Set the considerSummertime flag
 * 
 * @param considerSummertime 
 */
void NTPClientPlus::setConsiderSummertime(bool considerSummertime) 
{
    this->considerSummertime = considerSummertime;
}

/** 
 * @return considerSummertime flag 
 */
bool NTPClientPlus::getConsiderSummertime() const
{
    return considerSummertime;
}

/**
 * @brief For european summertime
 * 
 * @return true - if it is summertime,
 * @return false - if it is not
 */
bool NTPClientPlus::isSummertime()
{
    time_t rawtime = getUnixTime();
    struct tm* ti;
    ti = localtime(&rawtime);

    uint16_t year = ti->tm_year + 1900;
    uint8_t month = ti->tm_mon + 1;
    uint8_t monthDay = ti->tm_mday;
    uint8_t hour = ti->tm_hour;

    summertime = (month < 3 || getRawMonth() > 10);

    if (getRawMonth() < 3 || getRawMonth() > 10)
    {
        summertime = false;
        return summertime;
    }
    if (getRawMonth() > 3 && getRawMonth() < 10)
    {
        summertime = true;
        return summertime;
    }
    if (getRawMonth() == 3 && (getRawHour() + 24 * getRawMonthDay()) >= (2 + 24 * (31 - (5 * getRawYear() / 4 + 4) % 7)))
    {
        summertime = true;
        return true;
    }

    if (summertime && getRawMonth() == 10 && (getRawHour() + 24 * getRawMonthDay()) < (3 + 24 * (31 - (5 * getRawYear() / 4 + 1) % 7)))
    {
        summertime = true;
        return true;
    }
    else
    {
        summertime = false;
        return false;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void NTPClientPlus::sendNTPPacket()
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    udp->beginPacket(poolServerName, 123); // NTP requests are to port 123
    udp->write(packetBuffer, NTP_PACKET_SIZE);
    udp->endPacket();
}

uint32_t NTPClientPlus::getUnixTime() const
{
    return timeOffset + unixtime + ((millis() - lastUpdate) / 1000);
}

uint8_t NTPClientPlus::getRawHour() const
{
    return ((getUnixTime() % 86400L) / 3600);
}

uint8_t NTPClientPlus::getRawMonthDay() const
{
    time_t rawtime = getUnixTime();
    struct tm *ti;
    ti = localtime(&rawtime);
    int monthDay = (ti->tm_mday) < 10 ? 0 + (ti->tm_mday) : (ti->tm_mday);

    return monthDay;
}

uint8_t NTPClientPlus::getRawMonth() const
{
    time_t rawtime = getUnixTime();
    struct tm *ti;
    ti = localtime(&rawtime);
    int month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);

    return month;
}

uint16_t NTPClientPlus::getRawYear() const
{
    time_t rawtime = getUnixTime();
    struct tm *ti;
    ti = localtime(&rawtime);
    int year = ti->tm_year + 1900;

    return year;
}