#include "PCH.h"
#include "FileLinePeriod.h"
#include "DateTime.h"

namespace XE
{
	bool FFileLinePeriod::Check() const
	{
        static thread_local std::map<TTuple<const char *, int>, uint64_t> __last;
		uint64_t now = FDateTime::SteadyNano() / 1000 / 1000;

        uint64_t last = __last[Id];
        if (now < last)
            return false;

		if ((now - last) * Count < static_cast<uint64_t>(Period))
			return false;

        __last[Id] = now;
        return true;
	}
}
