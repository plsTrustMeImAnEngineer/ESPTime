#ifndef ESPTime_h
#define ESPTime_h

#include <Arduino.h>
#include <time.h>
#include <stdlib.h>
#include <Timezones.h>

extern "C" int setenv (const char *__string, const char *__value, int __overwrite);
extern "C" void tzset();

enum Month : uint8_t
{
    January = 1,
    February = 2,
    March = 3,
    April = 4,
    May = 5,
    June = 6,
    July = 7,
    August = 8,
    September = 9,
    October = 10,
    November = 11,
    December = 12
};

enum WeekDay : uint8_t
{
    Monday = 1,
    Tuesday = 2,
    Wensday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6,
    Sunday = 0
};

/**
 * @brief Struct to store all important time information
 */
struct Time
{
    uint16_t year = 0;
    uint8_t month = 0;
    uint16_t yearDay = 0;
    uint8_t monthDay = 0;
    uint8_t weekDay = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
    bool dst = false;
};

class ESPTime
{
    protected:
    time_t unixtime = 0;
    const char* timezone = TZ_Etc_UTC;

    public:
    ESPTime(time_t unixtime, const char* timezone = TZ_Etc_UTC);
    ESPTime(const Time& time, const char* timezone = TZ_Etc_UTC);
    ESPTime(const char* timezone = TZ_Etc_UTC);

    virtual void setTime(time_t unixtime);
    virtual bool setTime(Time time);

    virtual void setTimezone(const char* timezone);
    virtual const char* getTimezone() const;
    
    virtual time_t getUnixTime() const;

    virtual Time getTime() const;
    virtual uint16_t getYear() const;
    virtual uint8_t getMonth() const;
    virtual uint16_t getYearDay() const;
    virtual uint8_t getMonthDay() const;
    virtual uint8_t getWeekDay() const;
    virtual uint8_t getHour() const;
    virtual uint8_t getMinute() const;
    virtual uint8_t getSecond() const;
    virtual uint8_t isDST() const;

    virtual String getFormattedTime() const;
    virtual String getFormattedDate() const;
    virtual String getFormattedDateTime() const;
    
    virtual ESPTime& operator=(time_t unixtime);
    virtual ESPTime& operator=(const Time& time);
};

#endif