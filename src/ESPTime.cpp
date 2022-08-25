#include <ESPTime.h>

/**
 * @brief Construct a new ESPTime::ESPTime object
 * 
 * @param unixtime Unixtimestamp to initialize the object with
 * @param timezone The timezone-string (Recommendation: Use macro from Timezones.h)
 */
ESPTime::ESPTime(time_t unixtime, const char* timezone)
{
    _unixtime = unixtime;
    setTimezone(timezone);
}

/**
 * @brief Construct a new ESPTime::ESPTime object
 * 
 * @param time Time-struct to initialize the object with
 * @param timezone The timezone-string (Recommendation: Use macro from Timezones.h)
 */
ESPTime::ESPTime(const Time& time, const char* timezone)
{
    _unixtime = setTime(time);
    setTimezone(timezone);
}

/**
 * @brief Construct a new ESPTime::ESPTime object
 * 
 * @param timezone The timezone-string (Recommendation: Use macro from Timezones.h)
 */
ESPTime::ESPTime(const char* timezone)
{
    setTimezone(timezone);
}

/**
 * @brief Sets the Time
 * 
 * @param unixtime 
 */
void ESPTime::setTime(time_t unixtime)
{
    _unixtime = unixtime;
}

/**
 * @brief Sets the Time
 * 
 * @param time Time-struct containing all necessary time information
 * @return true, if Time-struct is valid
 */
bool ESPTime::setTime(Time time)
{
    if(time.year == 0 || time.month == 0 || time.monthDay == 0) return false;
    tm timeStruct;
    timeStruct.tm_year = time.year - 1900;
    timeStruct.tm_mon = time.month - 1;
    timeStruct.tm_mday = time.monthDay;
    timeStruct.tm_hour = time.hour;
    timeStruct.tm_min = time.minute;
    timeStruct.tm_sec = time.second;
    timeStruct.tm_isdst = time.dst;
    _unixtime = mktime(&timeStruct);
    return true;
}

/**
 * @brief Sets the Timezone
 * 
 * @param timezone 
 */
void ESPTime::setTimezone(const char* timezone)
{
    setenv("TZ", timezone, true);
    tzset();
    _timezone = timezone;
}

/** 
 * @return The current Timezone
 */
const char* ESPTime::getTimezone() const
{
    return _timezone;
}

/** 
 * @return The Unix-Timestamp
 */
time_t ESPTime::getUnixTime() const
{
    return _unixtime;
}

/** 
 * @return A Time-struct containing all the Time information
 */
Time ESPTime::getTime() const
{
    tm timeStruct = *localtime(&_unixtime);
    Time time;
    time.year = timeStruct.tm_year + 1900;
    time.month = timeStruct.tm_mon + 1;
    time.yearDay = timeStruct.tm_yday + 1;
    time.monthDay = timeStruct.tm_mday;
    time.weekDay = timeStruct.tm_wday;
    time.hour = timeStruct.tm_hour;
    time.minute = timeStruct.tm_min;
    time.second = timeStruct.tm_sec;
    time.dst = (bool)timeStruct.tm_isdst;
    return time;
}

/** 
 * @return The year
 */
uint16_t ESPTime::getYear() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_year + 1900;
}

/** 
 * @return The month 1 - 12
 */
uint8_t ESPTime::getMonth() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_mon + 1;
}

/** 
 * @return The day of the year 1 - 366
 */
uint16_t ESPTime::getYearDay() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_yday + 1;
}

/** 
 * @return The day of the month 1 - 31 
 */
uint8_t ESPTime::getMonthDay() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_mday;
}

/** 
 * @return The day of the week 0 - 6, 0 being Sunday 
 */
uint8_t ESPTime::getWeekDay() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_wday;
}

/** 
 * @return The hours sinnce midnight 0 - 23
 */
uint8_t ESPTime::getHour() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_hour;
}

/** 
 * @return The minutes since full hour 0 - 59 
 */
uint8_t ESPTime::getMinute() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_min;
}

/** 
 * @return The seconds since full minute 0 - 59 (0 - 61 for leap seconds) 
 */
uint8_t ESPTime::getSecond() const
{
    tm timeStruct = *localtime(&_unixtime);
    return timeStruct.tm_sec;
}

/** 
 * @return true, if Daylight Saving Time(Summertime) is active 
 */
uint8_t ESPTime::isDST() const
{
    tm timeStruct = *localtime(&_unixtime);
    return (bool)timeStruct.tm_isdst;
}

/** 
 * @return String containing time formatted like HH:MM:SS
 */
String ESPTime::getFormattedTime() const
{
    tm timeStruct = *localtime(&_unixtime);
    String hour = timeStruct.tm_hour < 10 ? "0" + String(timeStruct.tm_hour) : String(timeStruct.tm_hour);
    String minute = timeStruct.tm_min < 10 ? "0" + String(timeStruct.tm_min) : String(timeStruct.tm_min);
    String second = timeStruct.tm_sec < 10 ? "0" + String(timeStruct.tm_sec) : String(timeStruct.tm_sec);
    return hour + ":" + minute + ":" + second;
}

/** 
 * @return String containing date formatted like YYYY-MM-DD
 */
String ESPTime::getFormattedDate() const
{
    tm timeStruct = *localtime(&_unixtime);
    String year(timeStruct.tm_year + 1900);
    String month = timeStruct.tm_mon + 1 < 10 ? "0" + String(timeStruct.tm_mon + 1) : String(timeStruct.tm_mon + 1);
    String day = timeStruct.tm_mday < 10 ? "0" + String(timeStruct.tm_mday) : String(timeStruct.tm_mday);
    return year + "-" + month + "-" + day;
}

/** 
 * @return String containing date and time formatted like YYYY-MM-DD HH:MM:SS
 */
String ESPTime::getFormattedDateTime() const
{
    return getFormattedDate() + " " + getFormattedTime();
}

/**
 * @brief Assingns time using =
 * 
 * @param unixtime 
 */
ESPTime& ESPTime::operator=(time_t unixtime)
{
    setTime(unixtime);
    return *this;
}

/**
 * @brief Assigns time using =
 * 
 * @param time Time-struct containing all necessary time information
 */
ESPTime& ESPTime::operator=(const Time& time)
{
    setTime(time);
    return *this;
}