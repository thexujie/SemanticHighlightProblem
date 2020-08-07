#pragma once

#include "CoreInc.h"

namespace XE
{
	template<Integral T>
	class TConverter<T>
	{
	public:
		FStringView ToString(const T & Value) const
		{
			if constexpr (IsCharacterV<T>)
				return Format(Str("{}"), Char(Value));
			else
				return Format(Str("{}"), Value);
		}

		T FromString(FStringView String) const
		{
			if constexpr (IsCharacterV<T>)
				return T(String[0]);
			else
				return ToInterger<Char, T>(String.Data, String.Size, 0);
		}
	};

	template<FloatingPoint T>
	class TConverter<T>
	{
	public:
		FStringView ToString(const T & Value) const
		{
			return Format(Str("{}"), Value);
		}

		T FromString(FStringView String) const
		{
			return ToFloatingPoint<Char, T>(String.Data, String.Size);
		}
	};

	template<>
	class TConverter<char8_t>
	{
	public:
		FStringView ToString(const char8_t & Value) const
		{
			return Format(Str("{}"), Char(Value));
		}

		char8_t FromString(FStringView String) const
		{
			return char8_t(String[0]);
		}
	};

	template<typename T>
	class TConverter<TVec4<T>>
	{
	public:
		FStringView ToString(const TVec4<T> & Value) const
		{
			return Format(Str("{},{},{},{}"), Value.X, Value.Y, Value.Z, Value.W);
		}
		TVec4<T> FromString(FStringView String) const
		{
			TVector<FStringView> ComponentStrings = Split(String, Char(','));
			if (ComponentStrings.Size != 4)
				return {};

			TVec4<T> Value;
			Value.X = TConverter<T>().FromString(ComponentStrings[0]);
			Value.Y = TConverter<T>().FromString(ComponentStrings[1]);
			Value.Z = TConverter<T>().FromString(ComponentStrings[2]);
			Value.W = TConverter<T>().FromString(ComponentStrings[3]);
			return Value;
		}
	};
}
