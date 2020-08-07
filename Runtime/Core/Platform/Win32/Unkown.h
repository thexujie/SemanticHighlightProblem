#pragma once

#include "Win32Inc.h"

namespace XE::Win32
{
	template<typename T>
	class TUnkown : public T
	{
	public:
		TUnkown() = default;
		TUnkown(const TUnkown &) = default;
		TUnkown(TUnkown &&) noexcept = default;
		TUnkown & operator = (const TUnkown &) = default;
		virtual ~TUnkown() = default;

		HRESULT QueryInterface(const IID & riid, void ** ppvObject) override
		{
			if (riid == __uuidof(T))
				*ppvObject = this;
			else
				return E_NOINTERFACE;
			return S_OK;
		}

		ulong_t AddRef()
		{
			return Atomics::IncFetch(ReferCount);
		}

		ulong_t Release()
		{
			if (ReferCount == 0)
				throw EError::State;

			return Atomics::DecFetch(ReferCount);
		}
	public:
		ulong_t ReferCount = 1;
	};
	
}