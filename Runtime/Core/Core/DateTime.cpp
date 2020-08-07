#include "PCH.h"
#include "DateTime.h"

#define _CRT_NO_TIME_T
#include <time.h>
#include <sys/timeb.h>

namespace XE
{
    static int32_t _curr_timezone()
    {
        long timezone = 0;
        _get_timezone(&timezone);
        return (int32_t)timezone;
    }

    FDateTime::FDateTime()
    {

    }

    FDateTime::FDateTime(std::chrono::milliseconds msec)
    {
        TimeZone = _curr_timezone();
        int64_t m = msec.count();
        __time64_t sec = m / 1000 + TimeZone;
        tm ltime;
        _gmtime64_s(&ltime, &sec);
        Year = ltime.tm_year + 1900;
        Month = ltime.tm_mon + 1;
        Day = ltime.tm_mday;
        Hour = ltime.tm_hour;
        Minute = ltime.tm_min;
        Second = ltime.tm_sec;
        Millisecond = static_cast<int32_t>(m % 1000);
        Microsecond = 0;
        Nanosecond = 0;
    }

    FDateTime::FDateTime(std::chrono::microseconds mcsec)
    {
        TimeZone = _curr_timezone();
        int64_t cnt = mcsec.count();
        __time64_t sec = cnt / 1000000 + TimeZone;
        tm ltime;
        _gmtime64_s(&ltime, &sec);
        Year = ltime.tm_year + 1900;
        Month = ltime.tm_mon + 1;
        Day = ltime.tm_mday;
        Hour = ltime.tm_hour;
        Minute = ltime.tm_min;
        Second = ltime.tm_sec;
        Millisecond = static_cast<int32_t>((cnt / 1000) % 1000);
        Microsecond = cnt % 1000;
        Nanosecond = 0;
    }

    FDateTime::FDateTime(std::chrono::nanoseconds nsec)
    {
        TimeZone = _curr_timezone();
        int64_t cnt = nsec.count();
        __time64_t sec = cnt / 1000000000 + TimeZone;
        tm ltime;
        _gmtime64_s(&ltime, &sec);
        Year = ltime.tm_year + 1900;
        Month = ltime.tm_mon + 1;
        Day = ltime.tm_mday;
        Hour = ltime.tm_hour;
        Minute = ltime.tm_min;
        Second = ltime.tm_sec;
        Millisecond = static_cast<int32_t>((cnt / 1000000) % 1000);
        Microsecond = static_cast<int32_t>((cnt / 1000) % 1000);
        Nanosecond = static_cast<int32_t>((cnt / 1) % 1000);
    }


    FDateTime::FDateTime(int32_t _year, int32_t _month, int32_t _day, int32_t _hour, int32_t _minute, int32_t _second, int32_t _millisecond, int32_t _microsecond, int32_t _nanosecond) :
        Year(_year), Month(_month), Day(_day), Hour(_hour), Minute(_minute), Second(_second), Millisecond(_millisecond), Microsecond(_microsecond), Nanosecond(_nanosecond), TimeZone(_curr_timezone())
    {

    }

    FDateTime::FDateTime(const FDateTime & another)
    {
        Year = another.Year;
        Month = another.Month;
        Day = another.Day;
        Hour = another.Hour;
        Minute = another.Minute;
        Second = another.Second;
        Millisecond = another.Millisecond;
        Microsecond = another.Microsecond;
        Nanosecond = another.Nanosecond;
        TimeZone = another.TimeZone;
    }

    FDateTime & FDateTime::operator = (const FDateTime & another)
    {
        Year = another.Year;
        Month = another.Month;
        Day = another.Day;
        Hour = another.Hour;
        Minute = another.Minute;
        Second = another.Second;
        Millisecond = another.Millisecond;
        Microsecond = another.Microsecond;
        Nanosecond = another.Nanosecond;
        TimeZone = another.TimeZone;
        return *this;
    }

    int64_t FDateTime::msecs() const
    {
        tm ltime = {};
        ltime.tm_year = static_cast<int32_t>(Year - 1900);
        ltime.tm_mon = static_cast<int32_t>(Month - 1);
        ltime.tm_mday = static_cast<int32_t>(Day);
        ltime.tm_hour = static_cast<int32_t>(Hour);
        ltime.tm_min = static_cast<int32_t>(Minute);
        ltime.tm_sec = static_cast<int32_t>(Second);
        __time64_t sec = _mkgmtime64(&ltime);
        return (sec - TimeZone) * 1000 + Millisecond;
    }

    int64_t FDateTime::mcsecs() const
    {
        return msecs() * 1000 + Microsecond;
    }

    int64_t FDateTime::nsecs() const
    {
        return mcsecs() * 1000 + Nanosecond;
    }

    bool FDateTime::IsLeapyear(int32_t year)
    {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    EDayWeek FDateTime::DayWeek(int32_t year, int32_t month, int32_t day)
    {
        if (month < 1 || 12 < month)
            return EDayWeek::Monday;

        static int32_t DAY[] = { 6, 2, 1, 4, 6, 2, 4, 0, 3, 5, 1, 3 };
        year -= month < 3;
        return static_cast<EDayWeek>((year + year / 4 - year / 100 + year / 400 + DAY[month - 1] + day) % 7);
    }

    int32_t FDateTime::DayYear(int32_t year, int32_t month, int32_t day)
    {
        if (month < 1 || 12 < month)
            return 0;

        static int32_t DAY[] = { 1, 0, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7 };
        int32_t temp = (month - 1) * 30 + DAY[month - 1] + day;
        if (month > 2)
            temp -= IsLeapyear(year) ? 1 : 2;
        return temp;
    }

    FDateTime FDateTime::Now()
    {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return FDateTime(now_ms);
    }

    float64_t FDateTime::System()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        return now.time_since_epoch().count() * (float64_t)std::chrono::system_clock::period::num / (float64_t)std::chrono::system_clock::period::den;
    }

    uint64_t FDateTime::SystemNano()
    {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }

    float64_t FDateTime::Steady()
    {
        auto now = std::chrono::steady_clock::now();
        return now.time_since_epoch().count() * (float64_t)std::chrono::steady_clock::period::num / (float64_t)std::chrono::steady_clock::period::den;
    }

    uint64_t FDateTime::SteadyMilli()
    {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }
	
    uint64_t FDateTime::SteadyNano()
    {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }

    float64_t FDateTime::HighResolution()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return now.time_since_epoch().count() * (float64_t)std::chrono::high_resolution_clock::period::num / (float64_t)std::chrono::high_resolution_clock::period::den;
    }

    uint64_t FDateTime::HighResolutionNano()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }
}
