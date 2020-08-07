#pragma once

#include "CoreInc.h"

namespace XE
{
	class CORE_API FObject : public IRefer
	{
	public:
		FObject() = default;
		FObject(const FObject &) = default;
		FObject(FObject &&) noexcept = default;
		FObject & operator = (const FObject &) = default;
		virtual ~FObject() = default;

		uintx_t AddRef()
		{
			return Atomics::IncFetch(ReferCount);
		}

		uintx_t Release()
		{
			if (ReferCount == 0)
				throw EError::State;

			uintx_t Result = Atomics::DecFetch(ReferCount);
			if (Result == 0)
				DeleteUnSafe(this);
			return Result;
		}
	public:
		uintx_t ReferCount = 1;
	};
}