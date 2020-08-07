#pragma once

#include "StringView.h"

namespace XE
{
	class CORE_API FStringStream
	{
	public:
		virtual ~FStringStream() = default;
		virtual uintx_t Read(Char * Data, size_t Size) = 0;
		virtual uintx_t Write(const Char * Data, size_t Size) = 0;

		uintx_t Write(FStringView String)
		{
			return Write(String.Data, String.Size);
		}
		
		//template<typename T>
		//FStringStream & operator << (const T & Value)
		//{
		//	FStringView String = Format(Str("{}"), Value);
		//	Write(String.Data, String.Size);
		//	return *this;
		//}

		//FStringStream & operator << (const char * Text) &
		//{
		//	Write(reinterpret_cast<const Char*>(Text), Length(Text));
		//	return *this;
		//}

		//FStringStream && operator << (const char * Text) &&
		//{
		//	Write(reinterpret_cast<const Char *>(Text), Length(Text));
		//	return static_cast<FStringStream&&>(*this);
		//}

		//FStringStream & operator << (const char8_t * Text) &
		//{
		//	Write(reinterpret_cast<const Char*>(Text), Length(Text));
		//	return *this;
		//}

		//FStringStream && operator << (const char8_t * Text) &&
		//{
		//	Write(reinterpret_cast<const Char *>(Text), Length(Text));
		//	return static_cast<FStringStream &&>(*this);
		//}

		//FStringStream & operator << (FStringView String) &
		//{
		//	Write(String.Data, String.Size);
		//	return *this;
		//}

		//FStringStream && operator << (FStringView String) &&
		//{
		//	Write(String.Data, String.Size);
		//	return static_cast<FStringStream &&>(*this);
		//}

		//FStringStream & operator << (const FString & String) &
		//{
		//	Write(String.Data, String.Size);
		//	return *this;
		//}

		//FStringStream && operator << (const FString & String) &&
		//{
		//	Write(String.Data, String.Size);
		//	return static_cast<FStringStream &&>(*this);
		//}
	};
}
