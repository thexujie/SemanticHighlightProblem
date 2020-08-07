#pragma once

#include "Types.h"

namespace XE
{
	template <typename KeyT, typename ValueT>
	struct TPair
	{
		TPair() = default;
		TPair(const TPair &) = default;
		TPair(TPair &&) noexcept = default;
		TPair & operator=(const TPair &) = default;
		TPair & operator=(TPair &&) = default;
		auto operator <=>(const TPair &) const = default;

		TPair(const KeyT & FirstIn) : First(FirstIn), Second{} {}
		TPair(const KeyT & FirstIn, const ValueT & SecondIn) : First(FirstIn), Second(SecondIn) {}
		TPair(KeyT && FirstIn, ValueT && ValueIn) : First(Forward<KeyT>(FirstIn)), Second(Forward<ValueT>(ValueIn)) {}

		//template<typename KeyT2, typename ValueT2>
		//TPair(KeyT2 && FirstIn, ValueT2 && ValueIn) : First(Forward<KeyT2>(FirstIn)), Second(Forward<ValueT2>(ValueIn)) {}

		//template<typename KeyT2, typename ValueT2>
		//TPair(TPair<KeyT2, ValueT2> && Pair) : First(Forward<KeyT2>(Pair.First)), Second(Forward<ValueT2>(Pair.Second)) {}
		
		KeyT First;
		ValueT Second;
	};

	template<typename KeyT, typename ValueT>
	TPair<KeyT, ValueT> MakePair(const KeyT & First, const ValueT & Second)
	{
		return TPair<KeyT, ValueT>(First, Second);
	}

	template<typename KeyT, typename ValueT>
	struct THash<TPair<KeyT, ValueT>>
	{
		uintx_t operator()(const TPair<KeyT, ValueT> & Pair) const noexcept
		{
			uintx_t HashValue = HashJoin(Pair.First);
			HashValue = DEF_BASE_X * HashValue ^ HashJoin(Pair.Second);
			return HashValue;
		}
	};
}
