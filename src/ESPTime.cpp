#include <ESPTime.h>

ESPTime::ESPTime(time_t unixtime, const char* timezone)
{
    this->unixtime = unixtime;
    setTimezone(timezone);
}

ESPTime::ESPTime(const Time& time, const char* timezone)
{
    this->unixtime = setTime(time);
    setTimezone(timezone);
}

ESPTime::ESPTime(const char* timezone)
{
    setTimezone(timezone);
}

void ESPTime::setTime(time_t unixtime)
{
    this->unixtime = unixtime;
}

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
    unixtime = mktime(&timeStruct);
    return true;
}

void ESPTime::setTimezone(const char* timezone)
{
    setenv("TZ", timezone, true);
    tzset();
    this->timezone = timezone;
}

const char* ESPTime::getTimezone() const
{
    return timezone;
}

time_t ESPTime::getUnixTime() const
{
    return unixtime;
}

Time ESPTime::getTime() const
{
    tm timeStruct = *localtime(&unixtime);
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

uint16_t ESPTime::getYear() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_year + 1900;
}

uint8_t ESPTime::getMonth() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_mon + 1;
}

uint16_t ESPTime::getYearDay() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_yday + 1;
}

uint8_t ESPTime::getMonthDay() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_mday;
}

uint8_t ESPTime::getWeekDay() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_wday;
}

uint8_t ESPTime::getHour() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_hour;
}

uint8_t ESPTime::getMinute() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_min;
}

uint8_t ESPTime::getSecond() const
{
    tm timeStruct = *localtime(&unixtime);
    return timeStruct.tm_sec;
}

uint8_t ESPTime::isDST() const
{
    tm timeStruct = *localtime(&unixtime);
    return (bool)timeStruct.tm_isdst;
}

String ESPTime::getFormattedTime() const
{
    tm timeStruct = *localtime(&unixtime);
    String hour = timeStruct.tm_hour < 10 ? "0" + String(timeStruct.tm_hour) : String(timeStruct.tm_hour);
    String minute = timeStruct.tm_min < 10 ? "0" + String(timeStruct.tm_min) : String(timeStruct.tm_min);
    String second = timeStruct.tm_sec < 10 ? "0" + String(timeStruct.tm_sec) : String(timeStruct.tm_sec);
    return hour + ":" + minute + ":" + second;
}

String ESPTime::getFormattedDate() const
{
    tm timeStruct = *localtime(&unixtime);
    String year(timeStruct.tm_year + 1900);
    String month = timeStruct.tm_mon + 1 < 10 ? "0" + String(timeStruct.tm_mon + 1) : String(timeStruct.tm_mon + 1);
    String day = timeStruct.tm_mday < 10 ? "0" + String(timeStruct.tm_mday) : String(timeStruct.tm_mday);
    return year + "-" + month + "-" + day;
}

String ESPTime::getFormattedDateTime() const
{
    return getFormattedDate() + " " + getFormattedTime();
}

ESPTime& ESPTime::operator=(time_t unixtime)
{
    setTime(unixtime);
    return *this;
}

ESPTime& ESPTime::operator=(const Time& time)
{
    setTime(time);
    return *this;
}