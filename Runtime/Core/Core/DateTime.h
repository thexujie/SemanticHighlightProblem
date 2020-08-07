#pragma once

#include "CoreInc.h"

namespace XE
{
    enum class EDayWeek
    {
        Monday = 0,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };

    class CORE_API FDateTime
    {
    public:
        FDateTime();
        FDateTime(std::chrono::milliseconds msec);
        FDateTime(std::chrono::microseconds mcsec);
        FDateTime(std::chrono::nanoseconds nsec);
        FDateTime(int32_t _year, int32_t _month, int32_t _day, int32_t _hour, int32_t _minute, int32_t _second, int32_t _millisecond, int32_t _microsecond, int32_t _nanosecond);
        FDateTime(const FDateTime & another);
        FDateTime & operator =(const FDateTime & another);
        bool operator ==(const FDateTime & another) const { return nsecs() == another.nsecs(); }
        bool operator !=(const FDateTime & another) const { return !operator==(another); }
        int64_t msecs() const;
        int64_t mcsecs() const;
        int64_t nsecs() const;

        //! Year [-, -]
        int32_t Year;
        //! Month [1, 12]
        int32_t Month;
        //! Day [1, 31]
        int32_t Day;
        //! Hour [0, 23]
        int32_t Hour;
        //! Minute [0, 59]
        int32_t Minute;
        //! Second [0, 59]
        int32_t Second;
        //! Millisecond [0, 999]
        int32_t Millisecond;
        //! Microsecond [0, 999]
        int32_t Microsecond;
        //! Nanosecond [0, 999]
        int32_t Nanosecond;
        //! TimeZone [0, 999]
        int32_t TimeZone;

        static bool IsLeapyear(int32_t year);
        static EDayWeek DayWeek(int32_t year, int32_t month, int32_t day);
        static 	int32_t DayYear(int32_t year, int32_t month, int32_t day);

        static FDateTime Now();
        static float64_t System();
        static uint64_t SystemNano();
        static float64_t Steady();
        static uint64_t SteadyMilli();
        static uint64_t SteadyNano();
        static float64_t HighResolution();
        static uint64_t HighResolutionNano();
    };
}
